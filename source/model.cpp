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
int timeA = 0;
bool box = true;
bool p = true;

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
	transm[0] += .05;
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
}

SceneGraphNode* SceneGraphNode::getNode(int i) {
	return edges[i];
}

void SceneGraphNode::getPosition(GLfloat *pos) {
	pos[0] = transMatrix[0];
	pos[1] = transMatrix[1];
	pos[2] = transMatrix[2];
}

void SceneGraph::resetTimer() {
	timeA = 0;
	Z = 0;
}

void SceneGraph::drawBackground(GLuint* tex) {
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glPointSize(1);
	glBegin(GL_POINTS);
	for(int i = 0; i < 768; i++) {
		for(int j = 0; j < 1024; j++) {
			glTexCoord2f(0.0f + (1.0 / 1024.0 * j), 0.0f + (1.0 / 768.0 * i));
			glVertex3f(((float)(j) / 100.0 - 4), (float)(768 - i) / 100.0 - 4,  depth[j][i]);
		}
	}
	glEnd();
    glDisable(GL_TEXTURE_2D);	
}

void SceneGraph::drawBoundingBoxes() {
	glPushMatrix();
		glColor3f(1, 0, 0.0);
		glBegin( GL_LINES);
		for (int i = 0; i < boundingBoxes.boxes.size(); i++) {
			glColor3f(1, 1, 0.0);
			float minx = (float)boundingBoxes.boxes[i].minx / 100.0 - 4;
			float miny = (float)(768 - boundingBoxes.boxes[i].miny) / 100.0 - 4;
			float minz = boundingBoxes.boxes[i].minz;
			float maxx = (float)boundingBoxes.boxes[i].maxx / 100.0 - 4;
			float maxy = (float)(768 - boundingBoxes.boxes[i].maxy) / 100.0 - 4;
			float maxz = boundingBoxes.boxes[i].maxz;
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
		}
		
		glEnd();
		
	glPopMatrix();
}

void SceneGraph::drawScene() {
	glColor3f(1, 1, 1);
	if(bounding)
		drawBoundingBoxes();
	for(int i=0; i<graph.size(); i++) 
		graph[i]->drawNodeList();
}

float distance(float x1, float x2, float y1, float y2, float z1, float z2) {
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2) + pow(z2-z1, 2));
}

void SceneGraph::updateScene() {
}

void SceneGraph::init(int argc, char **argv) {
	char text[30];
	/* 
	 * Cow params
	 *
	 * float geomProps[7] = {-3, -0.8, 6.4, 0, 0, 0, 1.6};
	 */
	float geomProps[7] = {-3, -1.4, 5.8, 0, 0, 0, .5};
	Model *sphere = new Model("data/sphere.ply"); 
	sphere->setNormals();
	sphere->time = 0;
	SceneGraphNode *a = new SceneGraphNode(sphere, &geomProps[0]);
	a->addTexture(text);
	addModel(a);

  	ifstream f;
  	f.open("data/D.txt");
	for(int i = 0; i < 768; i++) {
		float x;
		for(int j = 0; j < 1024; j++) {
			f >> x;
			depth[j][i] = x / 25.0;	//253 is the max value
		}
	}
	f.close();
	boundingBoxes.computeBoundingBoxes(argc, argv, depth);
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
	eye[0] = 0 + X; 
	eye[1] = 0 + Y;
	eye[2] = 20.0 + Z;
	eye[3] = 1;
	multMatrix(&eye[0], rotmat);
	gluLookAt(eye[0], eye[1], eye[2], X, Y, Z, 0, 1, 0);
}
