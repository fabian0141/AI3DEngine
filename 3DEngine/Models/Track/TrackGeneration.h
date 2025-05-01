#include "../../GUI/GUI.h"
#include <vector>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

typedef float GLfloat;

class TrackGeneration : public GUIElement {
    public:
        TrackGeneration(GUI* gui);
        void click(const vec2 &mousePos, int action);
        void preDraw();
        void draw();

    private:
        const float BALL_RADIUS = 20;

        GUI *gui;
        GLuint vertexbuffer[3];
        GLuint debugVertexbuffer;

        vector<vec2> trackPoints[3];
        vector<vec2> interpolationPoints;

        const int roughPointsAmount = 10;

        void generateRoughPath(vector<vec2> pathPoints[3], bool randomize = false);
        void refinePath(vector<vec2> &pathPoints, int dir);
        vec2 getBallCenter(vec2 p1, vec2 p2, int dir);
        void createTrack(vector<vec2> pathPoints[3]);
        
        bool movePoint = false;
        int pointIdx;
};