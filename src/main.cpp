#include "shaders.h"
#include "texture.h"

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <iostream>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

//Number of pixels to process in fractal
#define CANVAS_WIDTH 256
#define CANVAS_HEIGHT 256

//Experimentally determined: runs best on my hardware
//If program crashes on boot, reduce this number
#define GROUP_SIZE 32

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(void) {
    if (!glfwInit())
        throw std::runtime_error("glfwInit() failed");

    //GLFW init
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sandpile", NULL, NULL);
    if (!window)
        throw std::runtime_error("Failed to create window");

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    //GLEW init
    glewExperimental = GL_TRUE;
    if(glewInit())
        throw std::runtime_error("glewInit() failed");

    //Disable Vsync
    glfwSwapInterval(0);

    GLuint computeProgram = CreateComputeProgram(GROUP_SIZE);
    Texture texture(CANVAS_WIDTH, CANVAS_HEIGHT);
    while (!glfwWindowShouldClose(window)) {
        //Switch active textures
        texture.SwitchTextures();

        //Run compute program with correct group count
        //Output texture is modified, render should wait for write to finish
        glUseProgram(computeProgram);
        glDispatchCompute(CANVAS_WIDTH/GROUP_SIZE, CANVAS_HEIGHT/GROUP_SIZE, 1);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        
        //Draw to screen
        texture.Draw();

        //Wait for next frame to be ready
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(computeProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}