#include <map>
#include <string>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

typedef unsigned int GLuint;

class Model;
class Camera;

struct Material {
    vec3 ambientColor;      //Ka
    vec3 diffuseColor;      //Kd
    vec3 specularColor;     //Ks
    float specularExponent; //Ns
    float dissolve;         //d
    float refraction;       //Ni
    int illumination;       //illum
    vec3 emmissive;         //Ke
};

struct MaterialGroup {
    Material* mat;
    vector<Model*> models;
};

class MaterialGroups {
    public:
        MaterialGroups(Camera* camera, GLuint programID);
        ~MaterialGroups();
        void addMaterial(string materialName, Material* mat);
        void addModel(string materialName, Model* model);
        void draw();
    private:
        map<string, MaterialGroup> matGroups;
        Camera* camera;
        GLuint programID;
};