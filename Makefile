all:
	g++ source/FastTrackball.cpp source/controller.cpp source/view.cpp source/main.cpp source/model.cpp -w -lglut -lGL -lGLU -lSOIL -o bin/exec 

run:
	g++ source/FastTrackball.cpp source/controller.cpp source/view.cpp source/main.cpp source/model.cpp -w -lglut -lGL -lGLU -lSOIL -o bin/exec 
	bin/./exec ${ARGS}
