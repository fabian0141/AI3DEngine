#pragma once

typedef unsigned int GLuint;

#include <string>

class ShaderLoader {
    public:
        static GLuint LoadShaders(const std::string vertexFilePath, const std::string fragmentFilePath,
            const std::string geometryFilePath = "");
};