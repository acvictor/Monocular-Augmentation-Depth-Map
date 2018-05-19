all:
	g++ -std=c++11 source/boundingBox.cpp source/FastTrackball.cpp source/controller.cpp source/view.cpp source/main.cpp source/model.cpp -w -lglui -lglut -lGL -lGLU -lSOIL `pkg-config --libs --cflags opencv` -o bin/exec 

run:
	g++ -std=c++11 source/boundingBox.cpp source/FastTrackball.cpp source/controller.cpp source/view.cpp source/main.cpp source/model.cpp -w -lglui -lglut -lGL -lGLU -lSOIL `pkg-config --libs --cflags opencv` -o bin/exec 
	bin/./exec ${ARGS}
