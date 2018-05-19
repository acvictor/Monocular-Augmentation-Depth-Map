#include <bits/stdc++.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <GL/glu.h>
#include "model.h"
#include "FastTrackball.h"

using namespace std;

class Vec {
	public:
		float x, y, z, a;
		Vec(float c, float d, float e, float f) {
			x = c;
			y = d;
			z = e;
			a = f;
		}
};

class Controller {
	public:
		int currentX, currentY, mState, change, which; 
		float Position[3];
		float data[3];
		bool headl, s1, s2;
		Trackball trackball;
		SceneGraph *model;
		void display(GLuint*);
		void lighting();
		void mode();
		void run(int, char**);
		void zoomIn();
		void zoomOut();
		void toggleBounding();
		void light();
		void moveX(float);
		void moveY(float);
		void rotate(float, float, float, float);
		Controller() {
			currentX = currentY = mState=-1;
			headl = s1 = s2 = false;
			change = which = 0;
		}
};
