#include "shaders.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string ReadFile(const char* name) {
    std::ifstream file(name);
    if(!file.good()) {
        std::cerr << "Could not locate shader: " << name << std::endl;
        throw std::runtime_error("File not found");
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint compileShader(GLuint type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    //Queries opengl for compile errors, gives sensible error logs
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar errorLog[512];
        glGetShaderInfoLog(shader, 512, NULL, errorLog);
        std::cerr << errorLog << std::endl;

        glDeleteShader(shader);
        throw std::runtime_error("Shader compile error");
    }
    return shader;
}

//Side effect: Disposes of shaders
GLuint linkProgram(GLuint shader0, GLuint shader1) {
    GLuint program = glCreateProgram();
    glAttachShader(program, shader0);
    glAttachShader(program, shader1);
    glLinkProgram(program);

    //Queries opengl for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        GLchar errorLog[512];
        glGetProgramInfoLog(program, 512, NULL, errorLog);
        std::cerr << errorLog << std::endl;

        glDeleteProgram(program);
        throw std::runtime_error("Program linking error");
    }

    glDeleteShader(shader0);
    glDeleteShader(shader1);
    return program;
}

GLuint CreateComputeProgram(int localSize) {
    std::string rawShader = ReadFile("shaders/compute.glsl");

    //Potentially unnecessary copy here, but it only runs once
    char *formattedShader = new char[rawShader.length()];
    std::sprintf(formattedShader, rawShader.c_str(), localSize, localSize);
    
    GLuint compute_shader = compileShader(GL_COMPUTE_SHADER, formattedShader);

    //0 used as dummy value
    return linkProgram(compute_shader, 0);
}

GLuint CreateTextureProgram() {
    std::string vertex_shader_src = ReadFile("shaders/draw_texture.vs");
    std::string fragment_shader_src = ReadFile("shaders/draw_texture.fs");

    GLuint vertex_shader = compileShader(GL_VERTEX_SHADER, vertex_shader_src.c_str());
    GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragment_shader_src.c_str());

    return linkProgram(vertex_shader, fragment_shader);
}