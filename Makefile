OBJS = src/main.cpp src/shaders.cpp src/texture.cpp

CC = g++ -std=c++11

COMPILER_FLAGS = -Wall


UBUNTU_FLAGS = -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
LINKER_FLAGS = -lGL -lGLU -lglut -lGLEW -lglfw

OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(UBUNTU_FLAGS) $(INC_PARAMS) -o $(OBJ_NAME)
