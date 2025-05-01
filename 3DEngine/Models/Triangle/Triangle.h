#include "../Model.h"

class Triangle : public Model {
    public:
        Triangle(Camera* camera);
        void draw();
    
    private:
        GLuint programID;
        Camera* camera;
};