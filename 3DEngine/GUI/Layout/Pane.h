#pragma once

#include "../GUI.h"
#include <GL/glew.h>

class Pane : public GUIElement {
public:
    Pane(vec2 pos = {});

    void click(const vec2& mousePos, int action) override;
    void preDraw(const vec2& parentSize) override;
    void draw() override;

private:
    GLuint VertexArrayID = 0;
    GLuint vertexbuffer = 0;
};
