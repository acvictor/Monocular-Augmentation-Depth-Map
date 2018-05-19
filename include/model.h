#include <bits/stdc++.h>
#include "GL/freeglut.h"
#include "GL/glui.h"
#include "GL/gl.h"
#include "boundingBox.h"
#include <GL/glu.h>

using namespace std;

class Color {
	public:
		float r, g, b;
		Color() {
			r = g = b = 0;
		}
		Color(float a, float x, float c) {
			r = a;
			g = x;
			b = c;
		}
};

class Vertex {
	public:
		float x, y, z, r, s;
		Vertex() {
			x = y = z = r = s = 0;
		}
		Vertex(float a, float b, float c) {
			x = a;
			y = b;
			z = c;
			r = s = 0;
		}
};

class Face {	
	public:
		vector<Vertex*> vertices;
		float normal[3];
		Face() {
			normal[0] = normal[1] = normal[2] = 0;
		};
};

class Model {
	public:
		char text[20];
		vector<Face*> faces;	
		GLuint tex;
		bool nflag;
		int time, angle, speed;
		float left, right, top, bottom, near, far, size, scale, dist;
		float mx, my, mz;
		int width, height;
		unsigned char *image;
		void computeTexSphere();
		void readTexture(char*);
		Model();
		Model(char*);
		virtual void setDirection(GLfloat *pos);
		virtual void nextPosition(float *transm, float *rotm, float* zoom);
		void read(char*);
		void print();
		void setNormals();
		void setTexture();
		virtual void render();	
		void drawBox();
};

class SceneGraphNode {
	public:
		Model* object;
		vector <SceneGraphNode*> edges;
		float transMatrix[3];
		float rotMatrix[3];
		float scale;
		SceneGraphNode(Model*);
		SceneGraphNode(Model*, float*);
		SceneGraphNode* getNode(int i);
		void appendModelNode(SceneGraphNode* sm);
		void drawNodeList();
		void addTexture(char *tex);
		void getPosition(GLfloat *pos);
		void drawModel();
		void setTransitions();
};

class SceneGraph {
	public:
		vector<int> lights;
		vector<SceneGraphNode*> graph;
		BoundingBox boundingBoxes;
		float eye[4];
		float transmat[6];
		float depth[1024][768];
		void resetTimer();
		int oldIndex, newIndex, tind;
		bool normalColour, lighting, changed, bounding, mode;
		float X, Y, Z;
		int time, changecam;
		void addModel(SceneGraphNode* sg);
		void init(int, char**);
		SceneGraph() {
			X = Y = Z = 0.0;
			eye[0] = eye[1] = eye[2] = time = 0;
			oldIndex = 0; newIndex = 0; tind = -1;
			normalColour = changed = mode = false;
			bounding = true;
			lighting = true;
		}
		void render();
		void drawBoundingBoxes();
		void drawBackground(GLuint*);
		void drawScene();
		void updateScene();
		void setCamera(float*, int);
};	
