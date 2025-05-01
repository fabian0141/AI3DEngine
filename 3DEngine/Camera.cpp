#include "Camera.h"

Camera::Camera(int width, int height) {
    projection = perspective(radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);
    
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    
    // Camera matrix
    mat4 view = lookAt(
        vec3(4,3,3), // Camera is at (4,3,3), in World Space
        vec3(0,0,0), // and looks at the origin
        vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    //View = mat4(1.0f);
    
    // Model matrix : an identity matrix (model will be at the origin)
    mat4 model = mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    mvp = projection * view * model;
}

GLfloat* Camera::getModelViewProjection() {
    return &mvp[0][0];
}

GLfloat* Camera::getModel() {
    return &model[0][0];
}

GLfloat* Camera::getView() {
    return &view[0][0];
}

GLfloat* Camera::getProjection() {
    return &projection[0][0];
}
vec3 Camera::getPosition() {
    return position;
}

void Camera::update(int width, int height, vec3 position, vec3 direction, vec3 up) {
    this->position = position;

    projection = perspective(radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);

    view = lookAt(position, position+direction, up);
    model = mat4(1.0f);
    mvp = projection * view * model;
}
