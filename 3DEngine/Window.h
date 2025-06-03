#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

class Model;
class Camera;
class MaterialGroups;
using namespace std;

class Window {
public:
    Window(int width, int height, shared_ptr<Camera> camera);
    Window(int width, int height);
    ~Window();

    void run(std::vector<Model*> models, MaterialGroups* matGroups);
    void run2D();
    void generate(int width, int height, unsigned int programID, void (*)(unsigned int));

private:
    unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window{nullptr, glfwDestroyWindow};
    shared_ptr<Camera> camera;

    void initWindow(int width, int height);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};