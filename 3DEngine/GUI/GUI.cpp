#include "GUI.h"
#include "../Loader/ShaderLoader.h"
#include "../Loader/TextureLoader.h"
#include <iostream>
#include <fstream>

static constexpr GLfloat vertices[] = {
   -1.0f, -1.0f,
    1.0f, -1.0f,
   -1.0f,  1.0f,
    1.0f,  1.0f
};


void GUI::generateFontMeta(GLuint programID) {
    GLuint fontTexLocation = glGetUniformLocation(programID, "fontTex");
    GLuint texID = TextureLoader::loadTGA("./Data/Textures/font.tga", false);
    glUniform1i(fontTexLocation, 0);

    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16I, 16, 16, 0, GL_RED_INTEGER, GL_SHORT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glUseProgram(programID);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(0);

    short pixels[16 * 16];
    glReadPixels(0, 0, 16, 16, GL_RED_INTEGER, GL_SHORT, pixels);

    std::ofstream out("./Data/Textures/font.meta", std::ios::binary);
    if (!out) {
        std::cerr << "Error: file could not be opened" << std::endl;
        std::exit(1);
    }

    for (int i = 15; i >= 0; --i)
        out.write(reinterpret_cast<char*>(&pixels[i * 16]), sizeof(short) * 16);

    out.close();
}

GLuint GUI::getProgramID(GUIProgram type) {
    auto index = static_cast<size_t>(type);
    if (guiProgramIDs[index] == 0) {
        switch (type) {
            case GUIProgram::TEXT:
                guiProgramIDs[index] = ShaderLoader::LoadShaders("./Data/Shader/Text.vert", "./Data/Shader/Text.frag", "./Data/Shader/Text.geom");
                break;
            case GUIProgram::BUTTON:
                guiProgramIDs[index] = ShaderLoader::LoadShaders("./Data/Shader/Button.vert", "./Data/Shader/Button.frag");
                break;
            case GUIProgram::TRACK_GENERATION:
                guiProgramIDs[index] = ShaderLoader::LoadShaders("./Data/Shader/TrackGeneration.vert", "./Data/Shader/TrackGeneration.frag");
                break;
        }
    }
    return guiProgramIDs[index];
}

GLuint GUI::getTextureID(GUITexture type) {
    auto index = static_cast<size_t>(type);
    if (guiTextureIDs[index] == 0) {
        switch (type) {
            case GUITexture::FONT:
                guiTextureIDs[index] = TextureLoader::loadTGA("./Data/Textures/font.tga");
                break;
            case GUITexture::FONT_META:
                guiTextureIDs[index] = TextureLoader::loadMETA("./Data/Textures/font.meta");
                break;
        }
    }
    return guiTextureIDs[index];
}

vector<int> GUI::getMetaData(GUITexture type) {
    auto index = static_cast<size_t>(type);
    if (guiMetaData[index].empty()) {
        switch (type) {
            case GUITexture::FONT_META:
                guiMetaData[index] = TextureLoader::loadMETAData("./Data/Textures/font.meta");
                break;
            default:
                break;
        }
    }
    return guiMetaData[index];
}

void GUI::updateEnvironment(int width, int height, vec2 contentPos, vec2 mousePos) {
    canvasSize = vec2(width, height) / 2.0f;
    this->contentPos = contentPos;
    this->mousePos = mousePos;
}
