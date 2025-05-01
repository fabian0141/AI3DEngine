#include "Intersection.h"

bool Intersection::isInsideQuad(vec2 point, vec2 lowerRight, vec2 upperLeft) {
    if (lowerRight.x <= point.x && point.x <= upperLeft.x) {
        if (lowerRight.y <= point.y && point.y <= upperLeft.y) {
            return true;
        }
    }
    return false;
}