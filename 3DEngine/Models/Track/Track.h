#include "../Model.h"
#include <vector>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

typedef float GLfloat;

class TrackGeneration : public Model {
    public:
        TrackGeneration(Camera* camera, GLuint programID);
        void draw();

    private:
        const float BALL_RADIUS = 0.2f;

        Camera* camera;
        GLuint programID;
        GLuint vertexbuffer[3];
        GLuint debugVertexbuffer;

        vector<vec3> trackPoints[3];
        vector<vec3> debugPoints;

        const int roughPointsAmount = 10;

        void generateRoughPath(vector<vec2> pathPoints[3]);
        void refinePath(vector<vec2> &pathPoints, int dir);
        vec2 getBallCenter(vec2 p1, vec2 p2, int dir);
        void createTrack(vector<vec2> pathPoints[3]);
};