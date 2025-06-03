#pragma once

#include "../GUI.h"
#include <string>
#include <array>

class Text : public GUIElement {
public:
    Text(glm::vec2 pos, int fontSize, std::string_view text);

    void click(const glm::vec2& mousePos, int action) override;
    void preDraw(const vec2& parentSize) override;
    void draw() override;

    ~Text();

private:
    int fontSize;
    std::array<int, 64> textInt{};
    int textSize = 0;

    GLuint vertexbuffer = 0;
    vector<int> fontWidths;
};
