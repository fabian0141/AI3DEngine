#include "../Model.h"
#include <vector>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;


class ObjModel : public Model {
    public:
        ObjModel(vector< vec3 >* vertices, vector< unsigned int >* indices, vector< vec2 >* uvs, vector< vec3 >* normals);

        ~ObjModel();
        void draw();

        ObjModel* getSubModel(vector< unsigned int >* indices);

    private:
        GLuint vertexbuffer;
        GLuint elementbuffer;
        GLuint normalbuffer;

        vector< vec3 >* vertices;
        vector< unsigned int >* indices;
        vector< vec2 >* uvs;
        vector< vec3 >* normals;

        bool isSubModel = false;

        ObjModel();
};