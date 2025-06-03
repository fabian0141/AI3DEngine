#pragma once
#include "GUIElement.h"
#include <array>
#include <memory>
#include <vector>
#include <GL/glew.h>


enum class GUIProgram {
    TEXT = 1,
    BUTTON,
    TRACK_GENERATION
};

enum class GUITexture {
    FONT = 1,
    FONT_META
};

class GUI {
public:
    vec2 canvasSize{};
    vec2 contentPos{};
    vec2 mousePos{};
    float contentZoom = 1.0f;


    static GUI& Get() {
        static GUI gui;
        return gui;
    };

    void setChild(unique_ptr<GUIElement> child) { this->child = move(child); };

    static void generateFontMeta(GLuint programID);
    GLuint getProgramID(GUIProgram type);
    GLuint getTextureID(GUITexture type);
    vector<int> getMetaData(GUITexture type);
    void click(const vec2& mousePos, int action) { child->click(mousePos, action); };
    void preDraw() { child->preDraw(canvasSize); };
    void draw() { child->draw(); };
    void updateEnvironment(int width, int height, vec2 contentPos, vec2 mousePos);

private:
    array<GLuint, 4> guiProgramIDs{};
    array<GLuint, 3> guiTextureIDs{};
    array<std::vector<int>, 3> guiMetaData{};
    unique_ptr<GUIElement> child;

    GUI() {};
    GUI(const GUI&) = delete;
    GUI& operator=(const GUI&) = delete;
};
