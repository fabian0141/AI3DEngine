#include "Button.h"
#include <GL/glew.h>
#include "../../Helper/Intersection.h"
#include <cstdio>


Button::Button(vec2 pos, GUI *gui) : GUIElement(1), gui(gui) {
    this->pos = pos;
}

void Button::click(const vec2 &mousePos, int action) {
    if (action == 1 && Intersection::isInsideQuad(mousePos, pos, pos + size)) {
        clickFunc();
    }
}

void Button::preDraw() {
    if (elements.size() == 1) {
        elements[0]->move(pos + vec2(20, 10));
        elements[0]->preDraw();
        size.x = elements[0]->size.x + 40;
        size.y = elements[0]->size.y + 20;
    }

    GLfloat vertices[] = {
        pos.x, pos.y + size.y,
        pos.x + size.x, pos.y + size.y,
        pos.x, pos.y,
        pos.x + size.x, pos.y
    };

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
}

void Button::draw() {
    GLuint programID = gui->getProgramID(GUI::BUTTON_PROGRAM);
    glUseProgram(programID);
    //glBindVertexArray(VertexArrayID);

    GLuint sizeID = glGetUniformLocation(programID, "canvasSize");
    glUniform2f(sizeID, gui->canvasSize.x, gui->canvasSize.y);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gui->getTextureID(GUI::TEXT_TEXTURE));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
    glDisableVertexAttribArray(0);
    if (elements.size() == 1) {
        elements[0]->draw();
    }
}