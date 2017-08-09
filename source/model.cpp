#include <bits/stdc++.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <unistd.h>
#include <GL/glu.h>
#include "../include/model.h"
#include "../include/SOIL.h"

SceneGraphNode *temp;

using namespace std;

float start[3], peak[3], stop[3], interval, ys, vs;
bool box = true;

void Model::setNormals() {
	float a[3], b[3], c[3], length;
	for(int i=0; i<faces.size(); i++) {
		a[0] = faces[i]->vertices[1]->x - faces[i]->vertices[0]->x;
		a[1] = faces[i]->vertices[1]->y - faces[i]->vertices[0]->y;
		a[2] = faces[i]->vertices[1]->z - faces[i]->vertices[0]->z;
		
		b[0] = faces[i]->vertices[2]->x - faces[i]->vertices[0]->x;
		b[1] = faces[i]->vertices[2]->y - faces[i]->vertices[0]->y;
		b[2] = faces[i]->vertices[2]->z - faces[i]->vertices[0]->z;
		
		// Cross Product
		c[0] = a[1] * b[2] - b[1] * a[2]; 
		c[1] = a[2] * b[0] - b[2] * a[0];
		c[2] = a[0] * b[1] - b[0] * a[1];
		
		// Normalization Factor
		length = sqrt((c[0] * c[0]) + (c[1] * c[1]) + (c[2] * c[2]));
		
		faces[i]->normal[0] = c[0] / length;
		faces[i]->normal[1] = c[1] / length;
		faces[i]->normal[2] = c[2] / length;
		
	}
}

void Model::computeTexSphere() {
	for(int i=0; i<faces.size(); i++) {
		for(int j=0; j<faces[i]->vertices.size(); j++) {
			faces[i]->vertices[j]->r = (atan2f(faces[i]->vertices[j]->z, faces[i]->vertices[j]->x)  + 3.14)/(2*3.14);
			faces[i]->vertices[j]->s = (atan2f(faces[i]->vertices[j]->z, faces[i]->vertices[j]->y*sin(2*3.14*faces[i]->vertices[j]->r)))/(2*3.14);
		}
	}
}

void Model::setDirection(GLfloat *pos) {	
}


Model::Model() {
	size = 0;
	scale = 1;
	angle = 0;
	speed = 1;
	nflag = false;
	tex = 0;
}

Model::Model(char *fname) {
	size = 0;
	scale = 1;
	angle = 0;
	speed = 1;
	nflag = false;
	tex = 0;
	read(fname);
}

void Model::nextPosition(float *transm, float *rotsm, float* scle) {
}	

void Model::read(char *argv) {
	vector<Vertex*> v;
	float minx, maxx, miny, maxy, minz, maxz;
	minx = miny = minz = 10000;
	maxx = maxy = maxz = -10000;
	FILE *file = fopen(argv,"r");
	int noOfv, noOfFaces;
	char buffer[100];
	if(file) {
		fgets(buffer, 100, file);
		while(strncmp("element vertex", buffer, strlen("element vertex")) != 0) {
			fgets(buffer, 100, file);
		}
		strcpy(buffer, buffer+strlen("element vertex"));
		sscanf(buffer, "%d", &noOfv);
					
		while(strncmp("element face", buffer, strlen("element face")) != 0) {
			fgets(buffer, 100, file);
		}
		strcpy(buffer, buffer+strlen("element face"));
		sscanf(buffer, "%d", &noOfFaces);
		
		while(strncmp("end_header", buffer, strlen("end_header")) != 0  ) {
			fgets(buffer, 100, file);			
		}
		
		for(int i=0; i<noOfv; i++) {
			fgets(buffer, 100, file);
			float a, b, c;
			sscanf(buffer, "%f %f %f", &a, &b, &c);
			minx = min(minx, a);
			miny = min(miny, b);
			minz = min(minz, c);
			maxx = max(maxx, a);
			maxy = max(maxy, b);
			maxz = max(maxz, c);
			Vertex* newV;
			if(strcmp(argv,"data/cobramkii.ply") == 0) {
				float d, e, f;
				nflag = true;
				sscanf(buffer, "%f %f %f", &d, &e, &f);
			}
			newV = new Vertex(a,b,c);
			v.push_back(newV);
		}
		if(size < maxx-minx)
		    	size = maxx-minx;
		if(size < maxy-miny)
			size = maxy-miny;
		if(size < maxz-minz)
		    	size = maxz-minz;
		scale = 2.0 / size;
		for (int i=0; i<noOfv; i++) {
		    	v[i]->x = scale * (v[i]->x - minx) - 1.0;
		    	v[i]->y = scale * (v[i]->y - miny) - 1.0;
			v[i]->z = scale * (v[i]->z - minz) - 1.0;
		}
		minx = miny = minz = 10000;
		maxx = maxy = maxz = -10000;
		for (int i=0; i<noOfv; i++) {
		    	minx = min(minx, v[i]->x);
		    	maxx = max(maxx, v[i]->x);
		    	miny = min(miny, v[i]->y);
		    	maxy = max(maxy, v[i]->y);
		    	minz = min(minz, v[i]->z);
			maxz = max(maxz, v[i]->z);
		}
		left = minx; right = maxx;
		top = maxy; bottom = miny;
		near = minz; far = maxz;
		for(int i=0; i<noOfFaces; i++) {
			int listSize;
			fscanf(file, "%d", &listSize);
			Face* newf = new Face();
			for(int j=0; j<listSize; j++) {
				int a;
				fscanf(file, "%d", &a);
				newf->vertices.push_back(v[a]);
			}
			faces.push_back(newf);
		}
		fclose(file);
		mx = -(maxx+minx)/2;
		my = -(maxy+miny)/2;
		mx = -(maxz+minz)/2;
		
	}
	else {
		printf("File cannot be opened\n");
	}
}

void Model::readTexture(char* texname) {
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image(texname, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
   	SOIL_free_image_data(image);
}

void Model::drawBox() {
	glPushMatrix();
		glColor3f(.75, .75, 0.0);
		glBegin( GL_LINES);
		float minx, maxx, miny, maxy, minz, maxz;
		minx = left;
		maxx = right;
		miny = bottom;
		minz = near;
		maxy = top;
		maxz = far;
	
		glVertex3f(minx, miny, minz);			
		glVertex3f(maxx, miny, minz);				
		glVertex3f(minx, maxy, minz);			
		glVertex3f(maxx, maxy, minz);			
		glVertex3f(minx, miny, minz);			
		glVertex3f(minx, maxy, minz);			
		glVertex3f(maxx, miny, minz);			
		glVertex3f(maxx, maxy, minz);			
		
		glVertex3f(minx, miny, maxz);			
		glVertex3f(maxx, miny, maxz);				
		glVertex3f(minx, maxy, maxz);			
		glVertex3f(maxx, maxy, maxz);			
		glVertex3f(minx, miny, maxz);			
		glVertex3f(minx, maxy, maxz);			
		glVertex3f(maxx, miny, maxz);			
		glVertex3f(maxx, maxy, maxz);			
		
		glVertex3f(minx, miny, minz);			
		glVertex3f(minx, miny, maxz);			
		glVertex3f(minx, maxy, minz);			
		glVertex3f(minx, maxy, maxz);			

		glVertex3f(maxx, miny, minz);			
		glVertex3f(maxx, miny, maxz);				
		glVertex3f(maxx, maxy, minz);			
		glVertex3f(maxx, maxy, maxz); 
		
		glEnd();
		
	glPopMatrix();
	glutPostRedisplay();
}

void Model::setTexture() {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex);
}

void Model::render() {
	for(int n=0; n<faces.size(); n++) {	
		glBegin(GL_POLYGON);		
		glColor3f(.75, .75, 1);
		for(int i=0; i<faces[n]->vertices.size(); i++) {	
			glNormal3f(faces[n]->normal[0], faces[n]->normal[1], faces[n]->normal[2]);
			glTexCoord2f(faces[n]->vertices[i]->r, faces[n]->vertices[i]->s);		
			glVertex3f(faces[n]->vertices[i]->x, faces[n]->vertices[i]->y, faces[n]->vertices[i]->z); 
		}
		glEnd();
	}
}

void Model::print() {
	for(int n=0; n<faces.size(); n++) {	
		for(int i=0; i<faces[n]->vertices.size(); i++) {	
			cout<<faces[n]->vertices[i]->x<<" "<<faces[n]->vertices[i]->y<<" "<<faces[n]->vertices[i]->z<<endl;
		}
	}
}

Light::Light() : Model() {
	GLfloat dif[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat position[] = {0, 0, 2, 1};
	GLfloat direction[] = {0, 0, 0};
	glLightfv(GL_LIGHT4, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT4, GL_SPECULAR, spec);
	glLightf(GL_LIGHT4,GL_SPOT_CUTOFF, 50);
	glLightf(GL_LIGHT4,GL_SPOT_EXPONENT, 0);
	glLightfv (GL_LIGHT4, GL_POSITION, position);
	glLightfv (GL_LIGHT4, GL_SPOT_DIRECTION, direction);
	glEnable(GL_LIGHT4);
}

void Light::render() {
}

void Light::setDirection(GLfloat *pos) {
	cout<<"Light: "<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<std::endl;
	GLfloat position[] = {0, 0, 2, 1};
	float no = sqrt(pos[0]*pos[0] + pos[1]*pos[1]+pos[2]*pos[2]);
	pos[0] /= no; pos[1] /= no; pos[2] /= no;
	glLightfv (GL_LIGHT4, GL_POSITION, position);
	glLightfv (GL_LIGHT4, GL_SPOT_DIRECTION, pos);
}

void Light::nextPosition(float *transm, float *rotsm, float* scle) { 
}

Sun::Sun(char *fname) : Model(fname) {
	time = 0; angle = 0;
	strcpy(text, "data/sun.jpg");
}

void Sun::render() {
	Model::render();
}

void Sun::nextPosition(float *transm, float *rotsm, float* scle) {
	float cosAngle, sinAngle;
	angle += speed;
	if(angle > 360.0) angle = 0.0;
	rotsm[1] = angle;
	time++; 
}

Planet::Planet(char *fname) : Model(fname) {
	time = 0; angle = 0;
	speed = 1;
	strcpy(text, "data/earth.jpg");
}

void Planet::render() {
	Model::render();
}

void Planet::nextPosition(float *transm, float *rotsm, float* scle){
	float cosAngle, sinAngle;
	angle += speed;
	if(angle > 360.0) angle = 0.0;
	cosAngle = cos(angle * 3.1416 / 180.0);
	sinAngle = sin(angle * 3.1416 / 180.0);
	rotsm[1] = angle;
	transm[0] = 3.0*sinAngle;
	transm[1] = 3.0*cosAngle;
	time++; 
}

Moon::Moon(char *fname) : Model(fname) {
	time = 0;
	strcpy(text, "data/moon.jpeg");
}

void Moon::render() {
	Model::render();
}

void Moon::nextPosition(float *transm, float *rotsm, float* scle) {
	float cosAngle, sinAngle;
	angle += speed;
	if(angle > 360.0) angle = 0.0;
	cosAngle = cos(angle * 3.1416 / 180.0);
	sinAngle = sin(angle * 3.1416 / 180.0);
	rotsm[1] = -angle;
	transm[0] = 3.5*sinAngle;
	transm[1] = 3.5*cosAngle;
	time++; 
}

Spaceship::Spaceship(char *fname) : Model(fname) {
	time = -1;
	strcpy(text, "data/m.jpeg");
}

void Spaceship::render() {
	Model::render();
}

void Spaceship::nextPosition(float *transm, float *rotsm, float* scle) {
	if(time!=-1) {
		float x, y;
		if(start[0]<stop[0]) 
			x = transm[0] + interval;
		else x = transm[0] - interval;
		y = ys + vs * (float)time - (.5 * .2 * (float)time * (float)time);
		transm[0] = x;
		transm[1] = y;
		rotsm[1] = angle;
	}
}

SceneGraphNode::SceneGraphNode(Model* o) {
	object = o;
	transMatrix[0] = 0;
	transMatrix[1] = 0;
	transMatrix[2] = 0;
	rotMatrix[0] = 0;
	rotMatrix[1] = 0;
	rotMatrix[2] = 0;
	scale = 1;
}  

SceneGraphNode::SceneGraphNode(Model* o, float* mat) {
	object = o;
	transMatrix[0] = mat[0];
	transMatrix[1] = mat[1];
	transMatrix[2] = mat[2];
	rotMatrix[0] = mat[3];
	rotMatrix[1] = mat[4];
	rotMatrix[2] = mat[5];
	scale = mat[6];
}

void SceneGraphNode::addTexture(char *tex){
	object->readTexture(tex);
}

void SceneGraphNode::appendModelNode(SceneGraphNode* sm) {
	edges.push_back(sm);
}

void SceneGraphNode::drawModel() {
	if (box)
		object->drawBox();
	glEnable(GL_TEXTURE_2D);
	object->setTexture();
	object->render();
	glDisable(GL_TEXTURE_2D);
}

void SceneGraphNode::setTransitions(){
	glTranslatef(transMatrix[0], transMatrix[1], transMatrix[2]);
	glRotatef(rotMatrix[0], 1, 0, 0);
	glRotatef(rotMatrix[1], 0, 1, 0);
	glRotatef(rotMatrix[2], 0, 0, 1);
	glScalef(scale, scale, scale);
}

void SceneGraphNode::drawNodeList() {
	glPushMatrix();
	setTransitions();
	drawModel();
	int i=0;
	for (i=0; i<edges.size(); i++) 
		edges[i]->drawNodeList();
	glPopMatrix();
	object->nextPosition(transMatrix, rotMatrix, &scale);
}

SceneGraphNode* SceneGraphNode::getNode(int i) {
	return edges[i];
}

void SceneGraphNode::getPosition(GLfloat *pos) {
	pos[0] = transMatrix[0];
	pos[1] = transMatrix[1];
	pos[2] = transMatrix[2];
}

void SceneGraph::drawSkyRight(GLuint* tex) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(100.0f, 100.0f, -11.0f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(100.0f, 100.0f, 11.0f);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(100.0f, -100.0f, 11.0f);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(100.0f, -100.0f, -11.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void SceneGraph::drawSkyLeft(GLuint *tex) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f, 100.0f, -11.0f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(-100.0f, 100.0f, 11.0f);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(-100.0f, -100.0f, 11.0f);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(-100.0f, -100.0f, -11.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void SceneGraph::drawSkyBack(GLuint* tex) {
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(100.0f, 100.0f, -11.0f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(-100.0f, 100.0f, -11.0f);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(-100.0f, -100.0f, -11.0f);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(100.0f, -100.0f, -11.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void SceneGraph::drawScene() {
	float pos[3];
	if(changed == true) {
		updateScene();
		temp->getPosition(&pos[0]);
	}
	else {
		if(oldIndex == 0) 
			pos[0] = pos[1] = pos[2] = 0;
		if(oldIndex == 1) 
			graph[1]->getPosition(&pos[0]);
		if(oldIndex == 2) 
			graph[1]->edges[0]->getPosition(&pos[0]);
	}	
	graph[2]->object->setDirection(&pos[0]);
	box = bounding;
	for(int i=0; i<graph.size(); i++) 
		graph[i]->drawNodeList();
}

float distance(float x1, float x2, float y1, float y2, float z1, float z2) {
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2) + pow(z2-z1, 2));
}

void SceneGraph::updateScene() {
	if(time == 0) {
		//starting position
		if(oldIndex == 0) {
			temp = graph[0]->edges.back();
			graph[0]->edges.pop_back();
			start[0] = graph[0]->transMatrix[0];
			start[1] = graph[0]->transMatrix[1];
			start[2] = graph[0]->transMatrix[2];
		}
		else if(oldIndex == 1) {
			temp = graph[1]->edges.back();
			graph[1]->edges.pop_back();
			start[0] = graph[1]->transMatrix[0];
			start[1] = graph[1]->transMatrix[1];
			start[2] = graph[1]->transMatrix[2];
		}
		else {
			temp = graph[1]->edges[0]->edges.back();
			graph[1]->edges[0]->edges.pop_back();
			start[0] = graph[0]->edges[0]->transMatrix[0];
			start[1] = graph[0]->edges[0]->transMatrix[1];
			start[2] = graph[0]->edges[0]->transMatrix[2];
		}
		
		//final position
		if(newIndex == 0) {
			stop[0] = graph[0]->transMatrix[0];
			stop[1] = graph[0]->transMatrix[1];
			stop[2] = graph[0]->transMatrix[2];
		}
		else if(newIndex == 1) {
			float cosAngle, sinAngle;
			float ang = (graph[1]->object->angle+(20*graph[1]->object->speed));
			if(ang > 360) ang = ang - 360;
			cosAngle = cos(ang * 3.1416 / 180.0);
			sinAngle = sin(ang * 3.1416 / 180.0);
			stop[0] = 3.0*sinAngle;
			stop[1] = 3.0*cosAngle;
			stop[2] = graph[1]->transMatrix[2];
		}
		else {
			float cosAngle, sinAngle;
			float ang = (graph[1]->edges[0]->object->angle+(20*graph[1]->edges[0]->object->speed));
			if(ang > 360) ang = ang - 360;
			cosAngle = cos(ang * 3.1416 / 180.0);
			sinAngle = sin(ang * 3.1416 / 180.0);
			stop[0] = 3.5*sinAngle;
			stop[1] = 3.5*cosAngle;
			stop[2] = graph[1]->edges[0]->transMatrix[2];
		}
		temp->transMatrix[0] = start[0];
		temp->transMatrix[1] = start[1]+2;
		temp->object->time = 0;
		ys = start[1];
		temp->object->dist = distance(stop[0], start[0], stop[1], start[1], stop[2], start[2]);
		interval = abs(start[0] - stop[0])/20;
		vs = (stop[1] - start[1] + 40) / 20;
		temp->object->angle = 0;
		addModel(temp);	
			
	}
	else if(time == 20) {
		temp->transMatrix[0] = 0; 
		temp->transMatrix[1] = 2; 
		temp->transMatrix[2] = -.4; 
		temp->scale = 1;
		graph.pop_back();		
		if(newIndex == 0) {
			graph[0]->appendModelNode(temp);
			graph[0]->edges[0]->addTexture("data/sun.jpg");
		}
		else if(newIndex == 1) {
			graph[1]->appendModelNode(temp);
			graph[1]->edges[1]->addTexture("data/earth.jpg");
		}
		else {
			graph[1]->edges[0]->appendModelNode(temp);
			graph[1]->edges[0]->edges[0]->addTexture("data/moon.jpeg");
		}	
		oldIndex = newIndex;
		changed = false;
		temp->object->time = -2;
	}
	time++;
	temp->object->time++;
}

void SceneGraph::init() {
	char text[30];
	strcpy(text, "data/sun.jpg");
	float geomProps[7] = {0, 0, 0, 0, 0, 0, 1.1};
	Sun *sun = new Sun("data/sphere.ply"); 
	sun->computeTexSphere();
	sun->setNormals();
	SceneGraphNode *a = new SceneGraphNode(sun, &geomProps[0]);
	a->addTexture(text);
	//sun->print();
	
	geomProps[0] = -3; geomProps[1] = 0; geomProps[2] = 0; geomProps[6] = .6;
	strcpy(text, "data/earth.jpg");
	Planet *planet = new Planet("data/sphere.ply");
	planet->computeTexSphere();
	planet->setNormals();
	SceneGraphNode *b = new SceneGraphNode(planet, &geomProps[0]);
	b->addTexture(text);
	
	geomProps[0] = -3.5; geomProps[1] = 0; geomProps[2] = 0; geomProps[6] = .3;
	strcpy(text, "data/moon.jpeg");
	Moon *moon = new Moon("data/sphere.ply");
	moon->computeTexSphere();
	moon->setNormals();
	SceneGraphNode *c = new SceneGraphNode(moon, &geomProps[0]);
	c->addTexture(text);
	
	b->appendModelNode(c);
	
	geomProps[0] = 0; geomProps[1] = 2; geomProps[2] = -.4; geomProps[3]=-90; geomProps[6] = 1;
	strcpy(text, "data/sun.jpg");
	Spaceship *ship = new Spaceship("data/cobramkii.ply");
	ship->computeTexSphere();
	ship->setNormals();
	SceneGraphNode *d = new SceneGraphNode(ship, &geomProps[0]);
	d->addTexture(text);
	
	temp = (SceneGraphNode*)ship;
	
	a->appendModelNode(d);
	
	Light *light = new Light();
	SceneGraphNode *e = new SceneGraphNode(light);

	time = 0;
	addModel(a);
	addModel(b);
	addModel(e);
}

void SceneGraph::addModel(SceneGraphNode* sg) {
	graph.push_back(sg);
}

void multMatrix(float *e, float *rot) {
	int i, j;
	float c[4];
	for (i=0; i<4; i++) {
		c[i] = 0.0;
		for (j=0; j<4; j++) {
			c[i] += e[j]*rot[4*j+i];
		}
	}
	for (i=0; i<3; i++)  {
		e[i] = c[i];
	}
}

void SceneGraph::setCamera(float *rotmat, int changeCam) {	
	if(changeCam == 0) {
		eye[0] = 0 + X; 
		eye[1] = 0+ Y;
		eye[2] = 8.0 + Z;
		eye[3] = 1;
		multMatrix(&eye[0], rotmat);
		gluLookAt(eye[0], eye[1], eye[2], X, Y, Z, 0, 1, 0);
	}
	
	if (changeCam == 1) {
		graph[1]->getPosition(&eye[0]);
		eye[2] -= 1;
		eye[3] = 1;
		gluLookAt(eye[0], eye[1], eye[2], X, Y, Z, 0, 1, 0);
	}
	
	if (changeCam == 2) {
		float y;
		if(changed == true) 
			temp->getPosition(&eye[0]);
		else {
			if(newIndex == 0) {
				graph[0]->getPosition(&eye[0]);
				y = graph[0]->rotMatrix[1];
				eye[0] += 1;
				eye[1] += 1.8;
				eye[2] += .4;
			}
			else if(newIndex == 1) {
				graph[1]->getPosition(&eye[0]);
				eye[0] += .5;
				eye[1] += .9;
				eye[2] += .2;
			}
			else {
				graph[1]->edges[0]->getPosition(&eye[0]);
				//eye[0] += .3;
				eye[1] += .3;
				//eye[2] += .13;
			}	
			multMatrix(&eye[0], rotmat);
		}		
		gluLookAt(eye[0], eye[1], eye[2], 1+X, 0+Y, 0+Z, 0, 1, 0);
	}  
	
	//cout<<eye[0]<<" "<<eye[1]<<" "<<eye[2]<<endl;
}
