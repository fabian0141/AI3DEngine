#include "../Model.h"
#include <vector>
#include <glm/glm.hpp>
#include <memory>

using namespace glm;
using namespace std;


class ObjModel : public Model {
public:
    ObjModel(vector<vec3> vertices, vector<unsigned int> indices, vector<vec2> uvs, vector<vec3> normals);
    ~ObjModel();
    void draw();
    unique_ptr<ObjModel> getSubModel(vector<unsigned int> indices);

private:
    ObjModel() {}; // Private default for submodels

    GLuint vertexArrayID = 0;
    GLuint vertexbuffer = 0;
    GLuint elementbuffer = 0;
    GLuint normalbuffer = 0;
    GLuint uvbuffer = 0;

    vector<vec3> vertices;
    vector<unsigned int> indices;
    vector<vec2> uvs;
    vector<vec3> normals;

    bool isSubModel = false;
};
    