#pragma once
#include <glm/glm.hpp>
#include <string>
#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

class TextureLoader {
public:
    // loadTGA returns GLuint texture or 0 on failure
    static GLuint loadTGA(const std::string& path, bool filter = true);

    // loadMETA returns GLuint texture or 0 on failure
    static GLuint loadMETA(const std::string& path);

    // loadMETAData returns std::optional with vector<int>, empty on failure
    static std::vector<int> loadMETAData(const std::string& path);
};
