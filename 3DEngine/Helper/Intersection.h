#include <glm/glm.hpp>
using namespace glm;

class Intersection {
    public:
        static bool  isInsideQuad(vec2 point, vec2 lowerRight, vec2 upperLeft);
};