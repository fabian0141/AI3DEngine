#include "../Model.h"

class Cube : public Model {
    public:
        Cube(Camera* camera);
        void draw();
    
    private:
        GLuint programID;
        Camera* camera;
};