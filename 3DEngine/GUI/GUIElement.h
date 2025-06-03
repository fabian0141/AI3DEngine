#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include <limits>
#include "GUI.h"


using namespace std;
using namespace glm;

class GUIElement {
public:
    vec2 size{100.0f, 60.0f};

    explicit GUIElement(int maxElements = numeric_limits<int>::max())
        : maxElements(maxElements) {}

    virtual ~GUIElement() = default;

    virtual void click(const vec2& mousePos, int action) = 0;
    virtual void preDraw(const vec2& parentSize) = 0;
    virtual void draw() = 0;

    bool addChildElement(unique_ptr<GUIElement> elem) {
        if (elements.size() < static_cast<size_t>(maxElements)) {
            elements.push_back(std::move(elem));

            return true;
        }
        return false;
    }

    void move(vec2 pos) { this->pos = pos; }

    void onClick(function<void()> func) { this->clickFunc = std::move(func); }

protected:
    vec2 pos{};
    vector<unique_ptr<GUIElement>> elements;
    function<void()> clickFunc;

private:
    int maxElements;
};
