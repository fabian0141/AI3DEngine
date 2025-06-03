#include "Button.h"
#include <GL/glew.h>
#include "../../Helper/Intersection.h"
#include <cstdio>

Button::Button(glm::vec2 pos)
    : GUIElement(1)
{
    this->pos = pos;
}

void Button::click(const glm::vec2& mousePos, int action) {
    if (action == 1 && Intersection::isInsideQuad(mousePos, pos, pos + size)) {
        if (clickFunc) clickFunc(); // safer call
    }
}

void Button::preDraw(const vec2& parentSize) {
    if (elements.size() == 1) {
        elements[0]->move(pos + glm::vec2(20, 10));
        elements[0]->preDraw(size);
        size.x = elements[0]->size.x + 40;
        size.y = elements[0]->size.y + 20;
    }

    const GLfloat vertices[] = {
        pos.x, pos.y + size.y,
        pos.x + size.x, pos.y + size.y,
        pos.x, pos.y,
        pos.x + size.x, pos.y
    };

    if (!VertexArrayID) {
        glGenVertexArrays(1, &VertexArrayID);
    }
    glBindVertexArray(VertexArrayID);

    if (!vertexbuffer) {
        glGenBuffers(1, &vertexbuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Button::draw() {
    GUI &gui = GUI::Get();
    GLuint programID = gui.getProgramID(GUIProgram::BUTTON);
    glUseProgram(programID);

    GLuint sizeID = glGetUniformLocation(programID, "canvasSize");
    glUniform2f(sizeID, gui.canvasSize.x, gui.canvasSize.y);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gui.getTextureID(GUITexture::FONT));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(0);

    if (!elements.empty()) {
        elements[0]->draw();
    }
}
