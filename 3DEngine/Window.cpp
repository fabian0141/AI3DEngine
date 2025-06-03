#include "Window.h"
#include <stdio.h>
#include <stdlib.h>

#include "Camera.h"
#include "Models/Model.h"
#include "MaterialGroups.h"
#include "GUI/GUI.h"

using namespace std;

constexpr float PI = 3.14159265f;
constexpr float ZOOM_IN_FACTOR = 1.02f;
constexpr float ZOOM_OUT_FACTOR = 0.98f;

Window::Window(int width, int height, shared_ptr<Camera> camera) : camera(move(camera)) {
    initWindow(width, height);
}

Window::Window(int width, int height) {
    initWindow(width, height);
}

Window::~Window() {
    glfwTerminate();
}

void Window::initWindow(int width, int height) {
    glewExperimental = true;
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // Antialiasing

    window = unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>
        (glfwCreateWindow(width, height, "OpenGL Window", nullptr, nullptr), glfwDestroyWindow);
    
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window.get());
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(-1);
    }

    glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Window::run(vector<Model*> models, MaterialGroups* matGroups) {
    if (!camera) {
        fprintf(stderr, "Camera is null in run()\n");
        return;
    }

    printf("Start drawing\n");

    vec3 position = vec3(0, 0, 5);
    float horizontalAngle = PI;
    float verticalAngle = 0.0f;
    float speed = 3.0f;
    float mouseSpeed = 0.02f;

    float lastTime = glfwGetTime();
    int nbFrames = 0;
    double lastFPSCall = glfwGetTime();

    do {
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        nbFrames++;
        if (currentTime - lastFPSCall >= 1.0) {
            printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            nbFrames = 0;
            lastFPSCall += 1.0;
        }

        int width, height;
        glfwGetWindowSize(window.get(), &width, &height);
        double xpos, ypos;
        double centerX = width / 2.0;
        double centerY = height / 2.0;

        glfwGetCursorPos(window.get(), &xpos, &ypos);
        glfwSetCursorPos(window.get(), centerX, centerY);

        horizontalAngle += mouseSpeed * deltaTime * float(centerX - xpos);
        verticalAngle += mouseSpeed * deltaTime * float(centerY - ypos);

        glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );

        glm::vec3 right(
            sin(horizontalAngle - PI / 2.0f),
            0,
            cos(horizontalAngle - PI / 2.0f)
        );

        glm::vec3 up = glm::cross(right, direction);

        if (glfwGetKey(window.get(), GLFW_KEY_W) == GLFW_PRESS) position += direction * deltaTime * speed;
        if (glfwGetKey(window.get(), GLFW_KEY_S) == GLFW_PRESS) position -= direction * deltaTime * speed;
        if (glfwGetKey(window.get(), GLFW_KEY_D) == GLFW_PRESS) position += right * deltaTime * speed;
        if (glfwGetKey(window.get(), GLFW_KEY_A) == GLFW_PRESS) position -= right * deltaTime * speed;

        camera->update(width, height, position, direction, up);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glViewport(0, 0, width, height);

        for (auto& model : models) model->draw();
        if (matGroups) matGroups->draw();

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }
    while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           !glfwWindowShouldClose(window.get()));
}

void Window::run2D() {
    GUI& gui = GUI::Get();

    gui.preDraw();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetMouseButtonCallback(window.get(), mouseCallback);
    glfwSetScrollCallback(window.get(), scrollCallback);

    vec2 position(0, 0);
    float speed = 500.0f;
    float lastTime = glfwGetTime();

    do {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        int width, height;
        glfwGetWindowSize(window.get(), &width, &height);

        if (glfwGetKey(window.get(), GLFW_KEY_W) == GLFW_PRESS) position.y += deltaTime * speed;
        if (glfwGetKey(window.get(), GLFW_KEY_S) == GLFW_PRESS) position.y -= deltaTime * speed;
        if (glfwGetKey(window.get(), GLFW_KEY_D) == GLFW_PRESS) position.x -= deltaTime * speed;
        if (glfwGetKey(window.get(), GLFW_KEY_A) == GLFW_PRESS) position.x += deltaTime * speed;

        double xpos, ypos;
        glfwGetCursorPos(window.get(), &xpos, &ypos);

        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, width, height);

        gui.updateEnvironment(width, height, position, vec2(xpos, ypos));
        gui.draw();

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }
    while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           !glfwWindowShouldClose(window.get()));
}

void Window::generate(int width, int height, unsigned int programID, void (*func)(unsigned int)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width, height);

    func(programID);

    glfwSwapBuffers(window.get());
    glfwPollEvents();
}

void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        GUI::Get().click(vec2(xpos, ypos), action);
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    GUI::Get().contentZoom *= (yoffset > 0) ? ZOOM_IN_FACTOR : ZOOM_OUT_FACTOR;
}
