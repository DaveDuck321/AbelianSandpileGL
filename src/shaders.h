#ifndef SHADER_TOOLS_H
#define SHADER_TOOLS_H

#include <GL/glew.h>

GLuint CreateComputeProgram(int localSize);
GLuint CreateTextureProgram();

#endif