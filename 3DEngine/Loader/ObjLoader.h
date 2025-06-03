#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

class MaterialGroups;

class ObjLoader {
public:
    static bool loadOBJ(const std::string& path, MaterialGroups* matGroups);

private:
    static bool loadMTL(const std::string& path, MaterialGroups* matGroups);
};
