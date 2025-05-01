#include "../GUI.h"
#include <string>


class Text : public GUIElement {
    public:
        Text(vec2 pos, int fontSize, string text, GUI *gui);
        void click(const vec2 &mousePos, int action);
        void preDraw();
        void draw();
    private:
        int fontSize;
        int textInt[64];
        int textSize;
        GUI *gui;
        GLuint vertexbuffer;
        int* fontWidths;
};