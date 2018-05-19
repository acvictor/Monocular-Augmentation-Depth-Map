#include "../include/controller.h"
#include "../include/SOIL.h"

#define pi 3.142857

using namespace std;

void Controller::lighting() {	
	glColor3f(1, 1, 1);
	GLfloat ambientColor[] = {0.6f, 0.6f, 0.6f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat lightPos0[] = {-2.5f, .6f, -2, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0); 
}		

void Controller::zoomIn() {
        model->Z -= .05f;
}

void Controller::zoomOut() {
        model->Z += .05f;
}

void Controller::toggleBounding() {
	if(model->bounding == 0)
        model->bounding = 1;
    else 
       model->bounding = 0;
}

void Controller::light() {
	if(model->lighting == false)
        	model->lighting = true;
        else 
        	model->lighting = false;
        if(model->lighting == true) 
        	glEnable(GL_LIGHT0);
	else 
		glDisable(GL_LIGHT0);
}	

void Controller::moveX(float x) {
	model->X += x;
}
  		   	
void Controller::moveY(float y) {
	model->Y += y;
}

void Controller::rotate(float currentX, float currentY, float x, float y) {
	int winX = glutGet(GLUT_WINDOW_WIDTH);
	int winY = glutGet(GLUT_WINDOW_HEIGHT);
	trackball.rotate((2.0*currentX - winX)/(float)winX, (winY - 2.0*currentY)/(float)winY, (2.0*x - winX)/(float)winX, (winY - 2.0*y)/(float)winY);
}

void Controller::display(GLuint* tex) {
	glLoadIdentity();
	glPushMatrix();							
		static float rotation_transform[4][4];					
		trackball.rotationMatrix(rotation_transform);	
		lighting();
		model->setCamera(&rotation_transform[0][0], change);
		model->drawBackground(tex);
		glEnable(GL_LIGHT0);
		model->drawScene();	
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	glPopMatrix();
	glutSwapBuffers();
}

void Controller::run(int argc, char **argv) {
	model->init(argc, argv);
}
