#include <glm/glm.hpp>
using namespace glm;

class Intersection {
public:
    static bool isInsideQuad(const vec2& point, const vec2& bottomRight, const vec2& topLeft);
};