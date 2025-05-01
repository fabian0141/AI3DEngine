#include <vector>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class GUIElement {
    public:
        vec2 size = vec2(100, 60);

        GUIElement(int maxElements = INT_MAX) : maxElements(maxElements) {};
        ~GUIElement() { for (auto elem : elements) delete elem; elements.clear(); };
        virtual void click(const vec2 &mousePos, int action) = 0;
        virtual void preDraw() = 0;
        virtual void draw() = 0;
        void addChildElement(GUIElement *elem) { if (maxElements > elements.size()) elements.push_back(elem);};
        void move(vec2 pos) { this->pos = pos; };
        void onClick(void (*clickFunc)()) { this->clickFunc = clickFunc; };
    protected:
        vec2 pos;
        vector<GUIElement*> elements;
        void (*clickFunc)();

    private:
        int maxElements;
};