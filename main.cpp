//Copyright github.com/charmingjohn

#include <cmath>
#include <vector>
#include <GL/glut.h>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include "Header.h"

#define PI 3.141592

using namespace std;

bool pause = false;
clock_t t;
ball *b;
bar *ba;
vector<brick*> br;
char* s = "abc";
int score = 0;
int level = 1;
int k = 0;
int select = 0;
float dist[17 * 6];


void play();


void* glutFonts[7] = {
	GLUT_BITMAP_9_BY_15,
	GLUT_BITMAP_8_BY_13,
	GLUT_BITMAP_TIMES_ROMAN_10,
	GLUT_BITMAP_TIMES_ROMAN_24,
	GLUT_BITMAP_HELVETICA_10,
	GLUT_BITMAP_HELVETICA_12,
	GLUT_BITMAP_HELVETICA_18
};

void write(char* text, float x=-0.45, float y=0, float r = 0, float g = 0, float b = 0, float a = 0,void * font=glutFonts[3])
{
	if (!text || !strlen(text)) return;
	bool blending = false;
	if (glIsEnabled(GL_BLEND)) blending = true;
	glEnable(GL_BLEND);
	glColor4f(r, g, b, a);
	glRasterPos2f(x, y);

	while (*text) {
		glutBitmapCharacter(font, *text);
		text++;
	}
	if (!blending) glDisable(GL_BLEND);
}



void scoreup(){
	write(_strdup(to_string(score).c_str()), 0.5, -0.2);
}

void levelup(float a=0.5,float b=0.2){
	write(_strdup(to_string(level).c_str()) , a, b);
}


void initShape(){
	
	srand(time(NULL));
	ba = new bar();
	b = new ball(level);
	for (int j = 0; j < level; j++){
		for (int i = 0; i <= 16; i++){
			int ran = rand() % 16;
			if (i == ran){//랜덤하게 빨간 벽돌 생성
				brick* o = new brick((float)(-0.94 + 0.07*i), 0.3 + j*0.06, 0, 1, 0, 0, ran % 2 + 2, 1);
				br.push_back(o);
			}
			else{
				brick* o = new brick((float)(-0.94 + 0.07*i), 0.3 + j*0.06, 0, 1 - (float)(i + 1) / 16, 1 - (float)(j + 1) / 6, 1 - (float)(i + j + 2) / 22);
				br.push_back(o);
			}
		}
	}
}

int closestbrick(){//공으로부터 가장 가까운 브릭을 찾는 함수
	float min = 100000;
	float result;
	int j = 200;
	for (int i = 0; i < br.size(); i++){
		result = (b->getX() - br[i]->getcentx())*(b->getX() - br[i]->getcentx()) + (b->getY() - br[i]->getcenty())*(b->getY() - br[i]->getcenty());
		dist[i] = result;//모든 브릭으로부터 거리를 구한후 dist[]에 저장한다.
	}
	for (int i = 0; i < br.size(); i++){
		if (dist[i] <= min){
			min = dist[i];//최소값을 찾음
			j = i; //최소값을 찾았다면 해당 index를 저장. 최단거리에 있는 브릭을 찾는다.
		}
	}
	return j;
}

void iti(int a){//빨간 벽돌을 부술 때 나오는 효과를 주기 위함.
	switch (a){
	case 0:
		ba->changebar(1.2);
		ba->changecol();
		break;
	case 1:
		ba->changebar(1.2);
		ba->changecol();
		break;
	case 2:
		ba->changebar(0.8);
		ba->changecol();
		break;
	case 3:
		ba->changebar(0.8);
		ba->changecol();
		break;
	default:
		break;
	}
}


void scoref(int i)
{
	if (br[i]->lf == 0){
		br[i]->nob();//브릭을 없앰
		if (br[i]->it == 1){//빨간 벽돌이면
			iti(rand() % 4);
			score = score + level * 5;//추가 점수를 줌
		}
		else
			score = score + level * 3;
	}
	br[i]->lf--;
}

clock_t initTime(){
	t = clock();
	return t;
}

void determinate(){
	delete b;
	int x = br.size();
	for (int i = 0; i < x; i++){
		br.pop_back();
	}
}

void idle(){
	
	if (k == 1 && !pause){ //game play mode
		b->move();	
		if ((b->getY() - b->getrad()) <= ba->getY() && (b->getY() + b->getrad()) >= ba->getY() - 0.05){
				if (b->getX() >= ba->getX() && b->getX() <= (ba->getX() + ba->geth()))
					b->dc2();
			}
			
		
			int i = closestbrick();
			if (i<200){
				if ((br[i]->getl() <= b->getX()) && (br[i]->getl() + 0.06 >= b->getX())){
					if ((b->getY() + b->getrad()) >= br[i]->getbot() && (b->getY() - b->getrad()) <= br[i]->getbot() + 0.05){
						b->dc2();//브릭의 위 혹은 아래에서 충돌할 경우 vel[1]를 반대로 바꿔줌.
						scoref(i);
					}
				}
			
				if (b->getY() >= br[i]->getbot() && b->getY() <= br[i]->getbot() + 0.05){
					if ((br[i]->getl() <= b->getrad() + b->getX()) && (br[i]->getl() + 0.06 >= b->getX() - b->getrad())){
						b->dc1();//브릭의 왼쪽 혹은 오른쪽에서 충돌할 경우 vel[0]를 반대로 바꿔줌.
						scoref(i);
					}
				}
		}
		if ((b->getY() + b->getrad()) >= 0.97){
			b->dc2();//왼쪽 벽
		}
		if (b->getX() >= 0.28 || b->getX() <= -0.97){
			b->dc1();//오른쪽 벽
		}

		
		if (b->getY() <= -1.3){
			clock_t h = clock();

			write("You Lose!!");
			glutSwapBuffers();
			while ((clock() - h) / CLOCKS_PER_SEC <= 2){

			}
			determinate();
			score = 0;
			k = 0;
		}
		if (br.size() != 0){
			int h = 0;
			for (int i = 0; i < br.size(); i++){
				if (br[i]->lf != -1){
					h = 1;
					break;
				}
			}
			if (h == 0){//브릭이 다 없어지면
				clock_t h = clock();
				if (level < 5){
					glutSwapBuffers();
					write("Level Clear!!");
					glutSwapBuffers();
					while ((clock() - h) / CLOCKS_PER_SEC <= 2){

					}
					determinate();
					level++;//레벨을 올림
					initShape();//올린 레벨로 다시 시작.
				}
				else{
					write("You WIN!!");//레벨 5까지 끝내면 You win
					glutSwapBuffers();
					while ((clock() - h) / CLOCKS_PER_SEC <= 2){

					}
					determinate();
					k = 0;
				}
			}
		}
	}
	glutPostRedisplay();
	
}

void start(){
	write("Arkanoid", -0.45, 0.5, 0, 0, 1);
	
	for (int i = 0; i < 3; i++){
		if (i == select)
			glColor3f(1, 0, 0);
		else
			glColor3f(1, 1, 0);
		glBegin(GL_QUADS);
		glVertex3f(-0.5, (float)(-i - 1) / 10 - 0.02, -0.01);
		glVertex3f(-0.1, (float)(-i-1)/10-0.02, -0.01);
		glVertex3f(-0.1, (float)-i / 10-0.03, -0.01);
		glVertex3f(-0.5, (float)-i/10-0.03, -0.01);
		glEnd();
	}
	write("play", -0.40, -0.1);
	write("select level", -0.40, -0.2);
	write("exit", -0.40, -0.3);
}



void play(){
	b->draw();
	ba->draw();
	for (int i = 0; i < br.size(); i++){
		br[i]->draw();
	}
	glColor3f(0.75, 0.75, 0.75);
	glBegin(GL_QUADS);
	glVertex3f(-1, 1, 0);
	glVertex3f(-0.98, 1, 0);
	glVertex3f(-0.98, -1, 0);
	glVertex3f(-1, -1, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-1, 1, 0);
	glVertex3f(0.3, 1, 0);
	glVertex3f(0.3, 0.97, 0);
	glVertex3f(-1, 0.97, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.29, 1, 0);
	glVertex3f(0.314, 1, 0);
	glVertex3f(0.314, -1, 0);
	glVertex3f(0.29, -1, 0);
	glEnd();

	write("Level", 0.5, 0.3);
	write("Score", 0.5, -0.1);
	scoreup();
	levelup();
}



void renderScene()
{
	glClearColor(1,1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.75, 0.75, 0.75);
	glBegin(GL_QUADS);
	glVertex3f(-1, 1, 0);
	glVertex3f(-0.98, 1, 0);
	glVertex3f(-0.98, -1, 0);
	glVertex3f(-1, -1, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-1, 1, 0);
	glVertex3f(0.3, 1, 0);
	glVertex3f(0.3, 0.97, 0);
	glVertex3f(-1, 0.97, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.29, 1, 0);
	glVertex3f(0.314, 1, 0);
	glVertex3f(0.314, -1, 0);
	glVertex3f(0.29, -1, 0);
	glEnd();
	
	
	switch (k){
	case 0: // start page
		start();
		break;
	case 1: // game play page
		play();
		break;
	case 2: // level select page
		write("Level");
		levelup(-0.45,-0.1);
		break;
	default:
		break;
	}
	
	glutSwapBuffers();
}
void SpecialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if ((ba->getX()+ba->geth() >= 0.2)&&(pause==false)){
			ba->loc(0.29-(ba->geth()*3/2));
		}
		else if (pause==false)
			ba->move(0.05, 0, 0);
		break;
	case GLUT_KEY_LEFT:
		if (ba->getX() <= -1&& pause==false){
			ba->loc(-1-(ba->geth()/2));
		}
		else if (pause==false)
			ba->move(-0.05, 0, 0);
		break;
		//fix
	case GLUT_KEY_UP:
		switch (k){
		case 0:
			if (select >0)
				select--;
				break;
		case 2:
			if (level < 5)
				level++;
			write("Level");
			levelup(-0.45, -0.1);
			glutSwapBuffers();
			break;
		default: break;
		}
		break;
	case GLUT_KEY_DOWN:
		switch (k){
		case 0:
			if (select <2)
				select++;
				break;
		case 2:
			if (level > 1)
				level--;
			write("Level");
			levelup(-0.45, -0.1);
			glutSwapBuffers();
			break;
		default: break;
		}
		break;
		//fix
	case GLUT_KEY_END:
		exit(0);
		break;
	default:
		break;
	}
	
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y)  {
	if (key == 'p'){
		pause = !pause;//p 누르면 게임이 잠시 멈춤
		
	}
	if (key == ' '){
		switch (k){
		case 0:
			switch (select){
			case 0: //play game level 1
				level = 1;
				initShape();
				k = 1;
				break;
			case 1: //select level
				k = 2;
				break;
			case 2://게임 종료
				exit(0);
				break;
			}
			break;
		case 2:
			initShape();
			k = 1;
			break;
		default:
			break;
		}
	}
	
	glutPostRedisplay();
} 


void main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 0);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("Arkanoid");
	glutSpecialFunc(SpecialKey);
	glutKeyboardFunc(processNormalKeys);
	initTime();	
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

}
