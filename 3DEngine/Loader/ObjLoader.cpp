#include "ObjLoader.h"
#include "../MaterialGroups.h"
#include "../Models/ObjModel/ObjModel.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <memory>

using namespace glm;
using namespace std;

struct MaterialIndices {
    string materialName;
    vector<unsigned int> indices;
};

static string getMTLPath(const string& objPath) {
    auto pos = objPath.find_last_of('.');
    if (pos == string::npos) return objPath + ".mtl";
    return objPath.substr(0, pos) + ".mtl";
}

bool ObjLoader::loadMTL(const string& mtlPath, MaterialGroups* matGroups) {
    ifstream file(mtlPath);
    if (!file.is_open()) {
        cerr << "Failed to open MTL file: " << mtlPath << '\n';
        return false;
    }

    unique_ptr<Material> currentMaterial = nullptr;
    string currentName;
    string line;

    while (getline(file, line)) {
        istringstream ss(line);
        string keyword;
        ss >> keyword;

        if (keyword == "newmtl") {
            if (currentMaterial && !currentName.empty()) {
                matGroups->addMaterial(currentName, move(currentMaterial));
            }
            ss >> currentName;
            currentMaterial = make_unique<Material>();
        } else if (currentMaterial) {
            if (keyword == "Ns") {
                ss >> currentMaterial->specularExponent;
            } else if (keyword == "Ka") {
                ss >> currentMaterial->ambientColor.x >> currentMaterial->ambientColor.y >> currentMaterial->ambientColor.z;
            } else if (keyword == "Kd") {
                ss >> currentMaterial->diffuseColor.x >> currentMaterial->diffuseColor.y >> currentMaterial->diffuseColor.z;
            } else if (keyword == "Ks") {
                ss >> currentMaterial->specularColor.x >> currentMaterial->specularColor.y >> currentMaterial->specularColor.z;
            } else if (keyword == "Ke") {
                ss >> currentMaterial->emmissive.x >> currentMaterial->emmissive.y >> currentMaterial->emmissive.z;
            } else if (keyword == "Ni") {
                ss >> currentMaterial->refraction;
            } else if (keyword == "d") {
                ss >> currentMaterial->dissolve;
            } else if (keyword == "illum") {
                ss >> currentMaterial->illumination;
                // On illumination line, add the material
                if (currentMaterial && !currentName.empty()) {
                    matGroups->addMaterial(currentName, move(currentMaterial));
                    currentMaterial = nullptr;  // prevent double add
                    currentName.clear();
                }
            }
        }
    }

    // Add last material if file ends without illumination keyword
    if (currentMaterial && !currentName.empty()) {
        matGroups->addMaterial(currentName, move(currentMaterial));
    }

    return true;
}

bool ObjLoader::loadOBJ(const string& path, MaterialGroups* matGroups) {
    if (!loadMTL(getMTLPath(path), matGroups)) {
        cerr << "Warning: MTL file not loaded or missing.\n";
        // We continue, OBJ can still be loaded without materials
    }

    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open OBJ file: " << path << '\n';
        return false;
    }

    vector<vec3> temp_vertices;
    vector<vec2> temp_uvs;
    vector<vec3> temp_normals;

    vector<unique_ptr<ObjModel>> models;

    unordered_map<string, vector<unsigned int>> materialToIndices;

    string currentMaterial;
    string line;

    while (getline(file, line)) {
        istringstream ss(line);
        string keyword;
        ss >> keyword;

        if (keyword == "v") {
            vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (keyword == "vt") {
            vec2 uv;
            ss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        } else if (keyword == "vn") {
            vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (keyword == "usemtl") {
            ss >> currentMaterial;
            // remove any file extension if present, e.g., "material.png" -> "material"
            auto dotPos = currentMaterial.find('.');
            if (dotPos != string::npos) currentMaterial = currentMaterial.substr(0, dotPos);
            if (!materialToIndices.contains(currentMaterial)) {
                materialToIndices[currentMaterial] = {};
            }
        } else if (keyword == "f") {
            // Parse face in the format v/vt/vn, 3 vertices per face (triangles)
            array<unsigned int, 3> vertexIndices{};
            array<unsigned int, 3> uvIndices{};
            array<unsigned int, 3> normalIndices{};

            // Support format: v/vt/vn for 3 vertices
            char slash;
            bool validFace = true;
            for (int i = 0; i < 3; ++i) {
                if (!(ss >> vertexIndices[i] >> slash >> uvIndices[i] >> slash >> normalIndices[i])) {
                    validFace = false;
                    break;
                }
            }

            if (!validFace) {
                cerr << "Error: unsupported face format or incomplete face data.\n";
                return false;
            }

            // Store indices relative to zero (OBJ indices start at 1)
            for (int i = 0; i < 3; ++i) {
                // Just store vertex indices (adjusted for zero base)
                materialToIndices[currentMaterial].push_back(vertexIndices[i] - 1);
            }
        }
    }

    // Now create ObjModels for each material
    for (auto& [materialName, indices] : materialToIndices) {
        // Create vectors for vertices, uvs, normals for this material
        auto vertices = vector<vec3>();
        auto uvs = vector<vec2>();
        auto normals = vector<vec3>();

        vertices.reserve(indices.size());
        uvs.reserve(indices.size());
        normals.reserve(indices.size());

        for (unsigned int idx : indices) {
            if (idx < temp_vertices.size()) {
                vertices.push_back(temp_vertices[idx]);
                if (!temp_uvs.empty()) {
                    uvs.push_back(idx < temp_uvs.size() ? temp_uvs[idx] : vec2{0, 0});
                }
                if (!temp_normals.empty()) {
                    normals.push_back(idx < temp_normals.size() ? temp_normals[idx] : vec3{0, 0, 0});
                }
            }
        }

        // Create ObjModel and add to MaterialGroups
        auto model = make_unique<ObjModel>(move(vertices), move(indices), move(uvs), move(normals));
        matGroups->addModel(materialName, move(model));
    }

    return true;
}
