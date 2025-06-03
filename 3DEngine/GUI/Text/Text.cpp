#include "Text.h"
#include <GL/glew.h>
#include <algorithm>

Text::Text(glm::vec2 pos, int fontSize, std::string_view text)
    : GUIElement(false), fontSize(fontSize)
{
    this->pos = pos;
    textSize = std::min(static_cast<int>(text.size()), 64);
    for (int i = 0; i < textSize; ++i) {
        textInt[i] = static_cast<int>(text[i]) - 33;
    }

    fontWidths = GUI::Get().getMetaData(GUITexture::FONT_META);

    int width = 0;
    for (int i = 0; i < textSize; ++i) {
        if (textInt[i] == -1) {
            width += 15;
        } else {
            width += fontWidths[textInt[i]] - (textInt[i] % 16) * 64;
        }
    }

    size = glm::vec2(width + 2 * textSize - 1, 64);
}

Text::~Text() {
    if (vertexbuffer != 0) {
        glDeleteBuffers(1, &vertexbuffer);
    }
}

void Text::click(const glm::vec2&, int) {
    // no-op
}

void Text::preDraw(const vec2& parentSize) {
    if (vertexbuffer == 0) {
        glGenBuffers(1, &vertexbuffer);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), &pos, GL_STATIC_DRAW);
}

void Text::draw() {
    GUI &gui = GUI::Get();
    GLuint programID = gui.getProgramID(GUIProgram::TEXT);
    glUseProgram(programID);

    glUniform2f(glGetUniformLocation(programID, "canvasSize"), gui.canvasSize.x, gui.canvasSize.y);
    glUniform1iv(glGetUniformLocation(programID, "text"), static_cast<GLsizei>(textInt.size()), textInt.data());
    glUniform1i(glGetUniformLocation(programID, "textSize"), textSize);
    glUniform1iv(glGetUniformLocation(programID, "fontMeta"), 256, fontWidths.data());

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gui.getTextureID(GUITexture::FONT));

    glDrawArrays(GL_POINTS, 0, 1);
    glDisableVertexAttribArray(0);
}
