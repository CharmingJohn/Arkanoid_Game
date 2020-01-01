//Copyright github.com/charmingjohn

#ifndef __class__
#define __class__

#include <cmath>
#include <vector>
#include <GL/glut.h>
#define PI 3.141592

class vec3{
public:
	vec3(){
		for (int i = 0; i < 3; i++)
			val[i] = 0;
	}
	float& operator[](const int i)
	{
		return val[i];//operator overloading
	}

	float val[3];
};

class object{
public: virtual void draw() = 0;

protected: vec3 color;
		   vec3 pos;
};

class brick :public object{//object class 상속
public:	int lf, it;
		brick(float a, float b, float c, float d, float e, float f, float k = 0,int x=0){
		pos[0] = a;
		pos[1] = b;
		pos[2] = c;
		color[0] = d;
		color[1] = e;
		color[2] = f;
		lf = k;
		it = x;
	}
	void draw(){
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex3f(pos[0], pos[1], pos[2]);
	glVertex3f(pos[0] + h, pos[1], pos[2]);
	glVertex3f(pos[0] + h, pos[1] + w, pos[2]);
	glVertex3f(pos[0], pos[1] + w, pos[2]);
	glEnd();
	}
	float getbot(){
		return pos[1];
	}
	float getl(){
		return pos[0];
	}
	void nob(){
		pos[0] += 100;
	}
	float getcentx(){
		return pos[0] + h / 2;
	}

	float getcenty(){
		return pos[1] + w / 2;
	}
private:float  h = 0.06, w = 0.05;
		
};

class ball : public object{//object 클래스 상속
public:ball(int level,int a=1,int b=0,int c=0){
	pos[0] = -0.45; pos[1] = -0.3; pos[2] = 0;
	color[0] = a; color[1] =b; color[2] =c;
	vel[0] = 1+(float)(level)/4; vel[1] = 1+(float)(level)/4;
}
	   void draw(){
		   glColor3f(color[0], color[1], color[2]);
		   glBegin(GL_TRIANGLE_FAN);
		   glVertex3f(pos[0], pos[1], pos[2]);
		   for (int i = 0; i <= 100; i++){
			   glVertex3f(pos[0] + cos(2 * PI / 100 * i)*rad, pos[1] + sin(2 * PI / 100 * i)*rad, pos[2]);
		   }
		   glEnd();
	   }
	   void move(){
		   pos[0] += 0.0012*vel[0];//공의 x좌표의 움직임을 표현. 속도 vel[0]를 곱한다.
		   pos[1] += 0.0017*vel[1];//공의 y좌표의 움직임을 표현. 속도 vel[1]을 곱한다.
	   }
	   void dc1(){
		   vel[0] = vel[0]*(-1);
	   }
	   void dc2(){
		   vel[1] = vel[1]*(-1);
	   }
	   float getX(){
		   return pos[0];
	   }
	   float getY(){
		   return pos[1];
	   }
	   float getrad(){
		   return rad;
	   }
	   void changerad(float y){
		   rad = rad*y;
	   }
	   void changecol(){
		   color[0] = float(rand()) / 10;
		   color[0] = float(rand()) / 10;
		   color[0] = float(rand()) / 10;

	   }
private: float rad = 0.013;
		 vec3 vel;
};

class bar : public object{
public:bar(){
	pos[0] = 0; pos[1] = -1+0.04; pos[2] = 0;
	color[0] = 0; color[1] = 0; color[2] = 1;
}
	   void draw(){
		   glColor3f(color[0], color[1], color[2]);
		   glBegin(GL_QUADS);
		   glVertex3f(pos[0], pos[1], pos[2]);
		   glVertex3f(pos[0] + h, pos[1], pos[2]);
		   glVertex3f(pos[0] + h, pos[1] + w , pos[2]);
		   glVertex3f(pos[0], pos[1] + w , pos[2]);
		   glEnd();

	   }
	   void move(float a, float b, float c){
		   pos[0] += a;
		   pos[1] += b;
		   pos[2] += c;
	   }
	   void loc(float a){
		   pos[0] = a + h/2;
	   }

	   float getY(){
		   return pos[1] + w;
	   }
	   float getX(){
		   return pos[0];
	   }
	   void changebar(float y){
		   h = h*y;
	   }
	   float geth(){
		   return h;
	   }
	   void changecol(){
		   color[0] = float(rand()) / 10;
		   color[0] = float(rand()) / 10;
		   color[0] = float(rand()) / 10;

	   }
	  
private:float h = 0.3, w = 0.02;
};

#endif