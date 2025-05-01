#include <glm/glm.hpp>
#include <vector>
#include <string>


using namespace glm;
using namespace std;

class MaterialGroups;


class ObjLoader {
    public:
        static bool loadOBJ( string path, MaterialGroups* matGroups);
    
    private:
        static bool loadMTL(string path, MaterialGroups* matGroups);
};