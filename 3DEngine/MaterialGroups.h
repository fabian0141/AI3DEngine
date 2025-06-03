#include <map>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

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
    unique_ptr<Material> mat;
    vector<unique_ptr<Model>> models;
};

class MaterialGroups {
    public:
        MaterialGroups(shared_ptr<Camera> camera, GLuint programID);
        void addMaterial(string materialName, unique_ptr<Material> mat);
        void addModel(string materialName, unique_ptr<Model> model);
        void draw();
    private:
        map<string, MaterialGroup> matGroups;
        shared_ptr<Camera> camera;
        GLuint programID;
};