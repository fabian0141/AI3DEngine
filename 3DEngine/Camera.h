#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>

using namespace glm;

class Camera {
    public:
        Camera(int width, int height);
        void update(int width, int height, vec3 position, vec3 direction, vec3 up);
        GLfloat* getModelViewProjection();
        GLfloat* getModel();
        GLfloat* getView();
        GLfloat* getProjection();
        vec3 getPosition();

    private:
        mat4 mvp;
        mat4 model;
        mat4 view;
        mat4 projection;
        vec3 position;
};