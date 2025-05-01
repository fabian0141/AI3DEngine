#include "GUI.h"
#include "../Loader/ShaderLoader.h"
#include "../Loader/TextureLoader.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <fstream>

static const GLfloat vertices[] = {
   -1.0f, -1.0f,
   1.0f, -1.0f,
   -1.0f,  1.0f,
   1.0f, 1.0f
};

GUI::GUI() {

}

GUI::~GUI() {
    for (int i = 0; i < 2; i++)
    {
        delete guiMetaData[i];
    }
}

void GUI::generateFontMeta(GLuint programID) {

    GLuint fontTexLocation = glGetUniformLocation(programID, "fontTex");
    GLuint texID = TextureLoader::loadTGA("./Data/Textures/font.tga", false);
    glUniform1i(fontTexLocation, 0);

    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create texture attachment
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16I, 16, 16, 0, GL_RED_INTEGER, GL_SHORT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Check framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glUseProgram(programID);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    2,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle

    short pixels[256];
    glReadPixels(0, 0, 16, 16, GL_RED_INTEGER, GL_SHORT, pixels);

    /*glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &framebuffer);*/
    glDisableVertexAttribArray(0);

    ofstream outdata; 
    int i; 
    outdata.open("./Data/Textures/font.meta", ios::binary | ios::out); 
    if( !outdata ) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    for (int i=15; i>=0; i--)
        for (int j = 0; j < 16; j++)
        outdata.write((char*)&pixels[i * 16 + j], sizeof(short));
    
    outdata.close();
}

GLuint GUI::getProgramID(int type) {
    if (guiProgramIDs[type] == 0) {
        switch (type)
        {
        case 1:
            guiProgramIDs[type] = ShaderLoader::LoadShaders( "./Data/Shader/Text.vert", "./Data/Shader/Text.frag", "./Data/Shader/Text.geom");
            break;
        case 2:
            guiProgramIDs[type] = ShaderLoader::LoadShaders( "./Data/Shader/Button.vert", "./Data/Shader/Button.frag");
            break;
        case 3:
            guiProgramIDs[type] = ShaderLoader::LoadShaders( "./Data/Shader/TrackGeneration.vert", "./Data/Shader/TrackGeneration.frag");
            break;    
        }
    }
    return guiProgramIDs[type];
}

GLuint GUI::getTextureID(int type) {
    if (guiTextureIDs[type] == 0) {
        switch (type)
        {
        case 1:
            guiTextureIDs[type] = TextureLoader::loadTGA( "./Data/Textures/font.tga");
            break;
        case 2:
            guiTextureIDs[type] = TextureLoader::loadMETA( "./Data/Textures/font.meta");
            break;
        }
    }
    return guiTextureIDs[type];
}

int* GUI::getMetaData(int type) {
    if (guiMetaData[type] == nullptr) {
        switch (type)
        {
        case 1:
            guiMetaData[type] = TextureLoader::loadMETAData( "./Data/Textures/font.meta");
            break;
        }
    }
    return guiMetaData[type];
}

void GUI::click(const vec2 &mousePos, int action) {
    //printf("Clicked at %f %f!\n", mousePos.x, mousePos.y);

    for (auto elem : elements) {
        elem->click(mousePos, action);
    } 
}


void GUI::preDraw() {
    for (auto elem : elements) {
        elem->preDraw();
    }
}

void GUI::draw() {
    for (auto elem : elements) {
        elem->draw();
    }
}

void GUI::updateEnvironment(int width, int height, vec2 contentPos, vec2 mousePos) {
    canvasSize = vec2(width, height) / 2.0f;
    this->contentPos = contentPos;
    this->mousePos = mousePos;
}