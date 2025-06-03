#include "../Model.h"

class Cube : public Model {
public:
    Cube(Camera* camera);
    ~Cube();
    void draw();

private:
    GLuint programID;
    Camera* camera;

    GLuint vao = 0;
    GLuint vertexBuffer = 0;
    GLuint uvBuffer = 0;
    GLuint textureID = 0;
    GLuint matrixID = 0;
    GLuint textureSamplerID = 0;
};
