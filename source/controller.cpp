#include "../include/controller.h"
#include "../include/SOIL.h"

#define pi 3.142857

using namespace std;

void Controller::pick() {
	//cout<<data[0]<<" "<<data[1]<<" "<<data[2]<<endl;
	if(model->mode == false) {
		if(abs(data[0] - data[1]) <=.001)
			model->newIndex = 2;
		else if(data[2]>data[0] and data[2]>data[1])
			model->newIndex = 1;
		else if(data[0]>data[1] and data[0]>data[2])
			model->newIndex = 0;
		if(model->oldIndex != model->newIndex) {
			model->changed = true;
			model->time = 0;
		}
		else model->changed = false;
	}
	else {
		if(abs(data[0] - data[1]) <=.001)
			model->tind = 3;
		else if(data[2]>data[0] and data[2]>data[1])
			model->tind = 2;
		else if(data[0]>data[1] and data[0]>data[2])
			model->tind = 1;
	}
}

void Controller::lighting() {	
	GLfloat ambientColor[] = {0.6f, 0.6f, 0.6f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat lightPos0[] = {-2.5f, .6f, -2, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0); 
	
	GLfloat light_position1[] = {0, 2, 0, 1.0};
    	GLfloat lightColor1[] = {1, 1, 1, 1};
    	GLfloat spot_position1[] = {0, 0, 0};
    	GLfloat spec_light1[] = {1,1,1,1};	
    	glLightfv(GL_LIGHT1, GL_SPECULAR, spec_light1);
    	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.0);
    	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spot_position1);

	glColor3f(1,1,1);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(20);
	glBegin(GL_POINTS);
		glVertex3f(-6, 8, -8);
	glEnd();
	
	
	GLfloat lightPosition2[] = {model->eye[0], model->eye[1], model->eye[2], 1.0};
	GLfloat spotPosition2[] = {-model->eye[0], -model->eye[1], -model->eye[2]};
    	GLfloat lightColor2[] = {1.0, 1.0, 1.0, 1.0};
    	GLfloat specLight2[] = {1, 1, 1, 1};
       	glLightfv(GL_LIGHT2, GL_SPECULAR, specLight2);
    	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10.0);
    	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);
    	glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,spotPosition2);
    	if(headl == true)
    		glEnable(GL_LIGHT2);
    	
    	GLfloat light_position3[] = {6, 8, 0, 1.0};
    	GLfloat lightColor3[] = {1, 1, 1, 1};
    	GLfloat spot_position3[] = {-6, -8, 0};
    	GLfloat spec_light3[] = {1,1,1,1};	
    	glLightfv(GL_LIGHT3, GL_SPECULAR, spec_light3);
    	glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor3);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 50.0);
    	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0.0);
    	glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,spot_position3);

	glColor3f(1,1,1);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(20);
	glBegin(GL_POINTS);
		glVertex3f(6, 8, -8);
	glEnd();
}		

void Controller::zoomIn() {
        model->Z -= .05f;
}

void Controller::zoomOut() {
        model->Z += .05f;
}

void Controller::spot1() {
	if(s1 == false)
		s1 = true;
	else s1 = false;
	if(s1 == true) 
		glEnable(GL_LIGHT1);
	else glDisable(GL_LIGHT1);
}

void Controller::spot2() {
	if(s2 == false)
		s2 = true;
	else s2 = false;
	if(s2 == true) 
		glEnable(GL_LIGHT3);
	else glDisable(GL_LIGHT3);
}

void Controller::bounding() {
	if(model->bounding == false)
        	model->bounding = true;
        else 
        	model->bounding = false;
}

void Controller::mode() {
	if(model->mode == false)
        	model->mode = true;
        else 
        	model->mode = false;
}

void Controller::headLight() {
	
	if(headl == false) 
		headl = true;
	else headl = false;
	if(headl == true) 
		glEnable(GL_LIGHT2);
	else glDisable(GL_LIGHT2);
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

void Controller::decreaseEarth() {
	if(model->graph[1]->object->speed > 1) {
		model->graph[1]->object->speed -= 1;
		model->graph[1]->edges[0]->object->speed -= 1;
	}
}

void Controller::increaseEarth() {
	model->graph[1]->object->speed += 1;
	model->graph[1]->edges[0]->object->speed += 1;
}

void Controller::decreaseSun() {
	if(model->graph[0]->object->speed > 1)
		model->graph[0]->object->speed -= 1;
}

void Controller::increaseSun() {
	model->graph[0]->object->speed += 1;
}

void Controller::display(GLuint* tex) {
	glLoadIdentity();
	glPushMatrix();							
		static float rotation_transform[4][4];					
		trackball.rotationMatrix(rotation_transform);	
		lighting();
		model->setCamera(&rotation_transform[0][0], change);
		model->drawSkyBack(tex);
		model->drawSkyRight(tex);
		model->drawSkyLeft(tex);
		model->drawScene();	
		glDisable(GL_LIGHTING);
	glPopMatrix();
	glutSwapBuffers();
}

void Controller::run() {
	model->init();
}
