#include <glm/glm.hpp>
#include <string>
#include <GL/glew.h>

using namespace glm;
using namespace std;


class TextureLoader {
    public:
        static GLuint loadTGA(string path, bool filter = true);
        static GLuint loadMETA(string path);
        static int* loadMETAData(string path);


};