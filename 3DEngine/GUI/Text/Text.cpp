#include "Text.h"
#include <GL/glew.h>

Text::Text(vec2 pos, int fontSize, string text, GUI *gui) : GUIElement(false), fontSize(fontSize), gui(gui) {
    this->pos = pos;
    textSize = text.size();
    for (int i = 0; i < textSize; i++)
    {
        textInt[i] = text[i] - 33;
    }

    fontWidths = gui->getMetaData(GUI::TEXT_META);
    int width = 0;
    for (int i = 0; i < textSize; i++ ) {
        if (textInt[i] == -1) {
            width += 15;
        } else {
            width += fontWidths[textInt[i]] - (textInt[i] % 16) * 64;
        }
    }
    size = vec2(width+2*textSize-1, 64);
}

void Text::click(const vec2 &mousePos, int action) {}

void Text::preDraw() {
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), &pos, GL_STATIC_DRAW);
}

void Text::draw() {
    GLuint programID = gui->getProgramID(GUI::TEXT_PROGRAM);
    glUseProgram(programID);

    GLuint sizeID = glGetUniformLocation(programID, "canvasSize");
    glUniform2f(sizeID, gui->canvasSize.x, gui->canvasSize.y);

    GLuint textID = glGetUniformLocation(programID, "text");
    glUniform1iv(textID, 64, textInt);

    GLuint textSizeID = glGetUniformLocation(programID, "textSize");
    glUniform1i(textSizeID, textSize);

    GLuint fontMetaLocation = glGetUniformLocation(programID, "fontMeta");
    glUniform1iv(fontMetaLocation, 256, fontWidths);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gui->getTextureID(GUI::TEXT_TEXTURE));

    glDrawArrays(GL_POINTS, 0, 1); 
    glDisableVertexAttribArray(0);
}