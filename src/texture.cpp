#include "texture.h"
#include "shaders.h"

#include <string.h>

static GLfloat vertices[] = {
    //Vertex Coords  //Texture coords
    -1.0f, -1.0f,    0.0f, 0.0f,
    -1.0f,  1.0f,    0.0f, 1.0f,
     1.0f, -1.0f,    1.0f, 0.0f,
    -1.0f,  1.0f,    0.0f, 1.0f,
     1.0f,  1.0f,    1.0f, 1.0f,
     1.0f, -1.0f,    1.0f, 0.0f,
};

void GenTexture(GLuint *id, int width, int height) {
    GLfloat* pData = new GLfloat[width*height];
    //Fills texture with 0s
    memset(pData, 0, width*height * sizeof(GLfloat));

    //Pixel at center gets large starting value
    //Higher values result in larger static fractals
    pData[ (width/2 + (height/2)*width) ] = (1<<16);

    glGenTextures(1, id);
    glBindTexture(GL_TEXTURE_2D, *id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Textures are only used for sand height: only need 1 red channel
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, pData);

    delete pData;
}

Texture::Texture(int width, int height) {
    //Create opengl textures
    GenTexture(&tex_computeIn, width, height);
    GenTexture(&tex_computeOut, width, height);
    
    //Load shaders to render texture
    drawProgram = CreateTextureProgram();
    
    //Populate buffers for drawing program
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Describes vertex coords
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    //Describes texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Texture::Draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(drawProgram);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Texture::SwitchTextures() {
    { //Switches texture IDs
        GLuint temp = tex_computeOut;
        tex_computeOut = tex_computeIn;
        tex_computeIn = temp;
    }
    //Allows drawing of output
    glBindTexture(GL_TEXTURE_2D, tex_computeOut);
    //Allows compute shader to write to texture
    glBindImageTexture(0, tex_computeIn, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    glBindImageTexture(1, tex_computeOut, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
}

Texture::~Texture() {
    glDeleteTextures(1, &tex_computeOut);
    glDeleteTextures(1, &tex_computeIn);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteProgram(drawProgram);
}