#include "../GUI.h"

class Button : public GUIElement {

    public:
        Button(vec2 pos, GUI *gui);
        void click(const vec2 &mousePos, int action);
        void preDraw();
        void draw();
    
    private:
        GUI *gui;
        GLuint VertexArrayID;
        GLuint vertexbuffer;
};