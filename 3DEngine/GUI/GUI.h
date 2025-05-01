#pragma once
#include "GUIElement.h"

typedef unsigned int GLuint;

class GUI : public GUIElement {
    public:
        static const int TEXT_PROGRAM = 1;
        static const int BUTTON_PROGRAM = 2;
        static const int TRACK_GENERATION_PROGRAM = 3;

        static const int TEXT_TEXTURE = 1;
        static const int TEXT_META = 1;

        vec2 canvasSize;
        vec2 contentPos;
        vec2 mousePos;
        float contentZoom = 1.0f;

        GUI();
        ~GUI();
    
        static void generateFontMeta(GLuint programID);
        GLuint getProgramID(int);
        GLuint getTextureID(int);
        int* getMetaData(int);
        void click(const vec2 &mousePos, int action);
        void preDraw();
        void draw();
        void updateEnvironment(int width, int height, vec2 contentPos, vec2 mousePos);
    private:
        GLuint guiProgramIDs[4] = {0, 0, 0, 0};
        GLuint guiTextureIDs[2] = {0, 0};
        int* guiMetaData[2] = {nullptr, nullptr};

};