#include "Intersection.h"

bool Intersection::isInsideQuad(const vec2& point, const vec2& bottomRight, const vec2& topLeft) {
    return bottomRight.x <= point.x && point.x <= topLeft.x &&
           bottomRight.y <= point.y && point.y <= topLeft.y;
}