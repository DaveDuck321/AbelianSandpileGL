#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GL/glu.h>

class Texture {
    private:
    //Two textures, one for reading, one for writing
    GLuint tex_computeIn, tex_computeOut;
    //Program for drawing textures
    GLuint drawProgram;
    //Buffers for drawing program 
    GLuint VAO, VBO;

    public:
    Texture(int width, int height);
    ~Texture();

    void Draw();
    void SwitchTextures();
};

#endif