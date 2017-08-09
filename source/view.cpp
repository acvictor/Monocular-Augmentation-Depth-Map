#include <bits/stdc++.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <GL/glu.h>
#include "../include/view.h"
#include "../include/SOIL.h"

using namespace std;

GLuint View::tex[] = {0, 0, 0};

void coords(int x, int y, double &xp, double &yp, double &zp) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport);	
	winX=(float)x;
	winY=(float)viewport[3] - (float)y;
	glReadBuffer( GL_COLOR_ATTACHMENT0 );
	glReadPixels(x, int(winY), 1, 1, GL_RGB, GL_FLOAT, &(View::controller->data));	
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &xp, &yp, &zp);
}


void View::myinit() {
	GLsizei winX = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei winY = glutGet(GLUT_WINDOW_HEIGHT);	
	glViewport(0, 0, winX, winY);
	glClearColor(0.0,0.0,0.0,1.0);
	glColor3f(1.0,0.0,0.0);
	
	unsigned char* image0;
	int width0, height0;
	
	glGenTextures(1, &tex[0]);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	image0 = SOIL_load_image("data/s.jpg", &width0, &height0, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width0, height0, 0, GL_RGB, GL_UNSIGNED_BYTE, image0);
	SOIL_free_image_data(image0);
	
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE); 
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();							
	gluPerspective(90.0, (float)winX /(float)winY, .1, 20.0);		
	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();	
}


void View::drawAxis() {
	glPushMatrix();
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-250.0, 0.0, -1.0f);
		glVertex3f(250.0, 0.0, -1.0f);		
	glEnd();
	glBegin( GL_LINES);
		glColor3f( 0.0, 0.0, 1.0);
		glVertex3f(0.0, -250.0, -1.0f);
		glVertex3f(0.0, 250.0, -1.0f);		
	glEnd();
	glPopMatrix();
}

void View::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	drawAxis();
	controller->display(tex);
	usleep(50000);
	glutPostRedisplay();
}

void View::reshape(GLsizei wt, GLsizei ht) {  
	if (ht == 0)
		ht = 1;								// Update Window's Height to New Height
	glViewport(0,0, wt, ht);						// Set the Viewing Window
	glMatrixMode(GL_PROJECTION); 						//Switch to setting the camera perspective
	glLoadIdentity(); 							//Reset the camera
	gluPerspective(90.0, (float)wt / (float)ht, .1, 20.0);		// Camera - ViewAngle, Viewport, nearZ, farZ
	glMatrixMode(GL_MODELVIEW);						// Entering Viewing Mode
	glLoadIdentity();							// Reset View Matrix
}

void View::keyboard(unsigned char key, int x, int y) {
	
   	switch (key) {
      		case 27:     
        	 	exit(0);
        	 	break;
        	 	
        	case '+':
        		controller->zoomIn();
       		   	glutPostRedisplay();
        		break;
        		
        	case '-':
        		controller->zoomOut();
       		   	glutPostRedisplay();
        		break;
        		
        	case 'l':
        		controller->light();
        		glutPostRedisplay();
        		break;
        		
        	case 'h':
        		controller->headLight();
        		glutPostRedisplay();
        		break;
        		
        	case 'v':
        		controller->mode();
        		glutPostRedisplay();
        		break;
        	
        	case 'b':
        		controller->bounding();
        		glutPostRedisplay();
        		break;
        		
        	case '1':
        		controller->spot1();
        		glutPostRedisplay();
        		break;
        		
        	case '2':
        		controller->spot2();
        		glutPostRedisplay();
        		break;
        	
        	case '3':
        		controller->increaseEarth();
        		glutPostRedisplay();
        		break;
        	
        	case '4':
        		controller->increaseSun();
        		glutPostRedisplay();
        		break;
        	
        	case '5':
        		controller->decreaseEarth();
        		glutPostRedisplay();
        		break;
        	
        	case '6':
        		controller->decreaseSun();
        		glutPostRedisplay();
        		break;
        	
        	case 'c':
        		controller->change = 1;
        		glutPostRedisplay();
        		break;
        	
        	case 'd':
        		controller->change = 0;
        		glutPostRedisplay();
        		break;
        		
        	case 'e':
        		controller->change = 2;
        		glutPostRedisplay();
        		break;
        		
        	default:
        		break;       
   	}
}

void View::mouseMotion(int x, int y) {
	if (View::controller->mState == 0) {
		View::controller->rotate(View::controller->currentX, View::controller->currentY, x, y);
		glutPostRedisplay();
    	}
  	View::controller->currentX = x;
    	View::controller->currentY = y;
}


void View::mouse(int button, int state, int x, int y) {
	double nx, ny, nz;
	coords(x, y, nx, ny, nz);
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		View::controller->mState = 0;				// Update Mouse State when Left Button pressed
                View::controller->currentX = x;
                View::controller->currentY = y;
        }	
    	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        	View::controller->mState = 1;				
		View::controller->rotate(0, 0, 0, 0);		      
       		glutPostRedisplay();
	}
	else if(button == GLUT_RIGHT_BUTTON and state == GLUT_UP) {
		controller->pick();
		glutPostRedisplay();
	}
}


void View::specialKeys(int key, int x, int y) {
	if(key == GLUT_KEY_LEFT) 
		controller->moveX(-0.1);
		
	else if(key == GLUT_KEY_UP)
		controller->moveY(0.1);
		
	else if(key == GLUT_KEY_RIGHT)
		controller->moveX(0.1);
	
	else if(key == GLUT_KEY_DOWN)
		controller->moveY(-0.1);
	
	glutPostRedisplay();
}

void View::init(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700,700);
	glutCreateWindow("3D");
	View::myinit();
	controller->run();
	glutDisplayFunc(View::display); 
	glutReshapeFunc(View::reshape);       
	glutKeyboardFunc(View::keyboard);
	glutSpecialFunc(View::specialKeys);
	glutMotionFunc(View::mouseMotion); 
	glutMouseFunc(View::mouse);
	//glutTimerFunc(1000, update, 0);
	glutMainLoop();
}
