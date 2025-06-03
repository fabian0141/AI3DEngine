#include "Pane.h"
#include <GL/glew.h>
#include "../../Helper/Intersection.h"
#include <cstdio>

Pane::Pane(vec2 pos) : GUIElement()
{
    this->pos = pos;
}

void Pane::click(const glm::vec2& mousePos, int action) {
    if (action == 1 && Intersection::isInsideQuad(mousePos, pos, pos + size)) {
        for_each(elements.begin(), elements.end(), [&](unique_ptr<GUIElement> &elem) { elem->click(mousePos, action); });
    }
}

void Pane::preDraw(const vec2& parentSize) {
    size = parentSize;
    for_each(elements.begin(), elements.end(), [&](unique_ptr<GUIElement> &elem) { elem->preDraw(size); });
}

void Pane::draw() {
    for_each(elements.begin(), elements.end(), [](unique_ptr<GUIElement> &elem) { elem->draw(); });
}
