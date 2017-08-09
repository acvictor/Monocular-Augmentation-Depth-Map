#include <bits/stdc++.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <GL/glu.h>
#include "controller.h"
#include <unistd.h>

using namespace std;

class View {
	public:
		static SceneGraph * model;
		static GLuint tex[3];
		static Controller * controller;
		static void init(int, char**);
		static void myinit();
		static void drawAxis();
		static void display();
		static void keyboard(unsigned char, int, int);
		static void mouse(int, int, int, int);
		static void mouseMotion (int, int);
		static void reshape(GLsizei, GLsizei);
		static void specialKeys(int, int, int);
};
	
