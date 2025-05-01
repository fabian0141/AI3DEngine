#include <vector>
#include "GUI/GUI.h"

class GLFWwindow;
class Model;
class Camera;
class MaterialGroups;

class Window {
    public:
        Window(int width, int height, Camera* camera);
        Window(int width, int height);

        void run(std::vector<Model*> models, MaterialGroups* matGroups);
        void run2D(GUI &gui);
        void generate(int width, int height, unsigned int programID, void (*)(unsigned int));

    private:
        GLFWwindow* window;
        Camera* camera;
        static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

};