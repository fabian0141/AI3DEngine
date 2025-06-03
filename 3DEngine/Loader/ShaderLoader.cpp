#include "ShaderLoader.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <optional>

GLuint ShaderLoader::LoadShaders(const std::string vertexFilePath,
                                const std::string fragmentFilePath,
                                const std::string geometryFilePath)
{
    auto readFile = [](const std::string& filePath) -> std::string {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open shader file: " << filePath << '\n';
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    };

    const auto vertexShaderCodeOpt = readFile(vertexFilePath);
    if (vertexShaderCodeOpt.empty()) return 0;

    const auto fragmentShaderCodeOpt = readFile(fragmentFilePath);
    if (fragmentShaderCodeOpt.empty()) return 0;

    std::string geometryShaderCodeOpt;
    if (!geometryFilePath.empty()) {
        geometryShaderCodeOpt = readFile(geometryFilePath);
        if (geometryShaderCodeOpt.empty()) return 0;
    }

    auto compileShader = [](GLuint shaderID, const std::string& source, const std::string& shaderName) -> bool {
        const char* sourcePtr = source.c_str();
        glShaderSource(shaderID, 1, &sourcePtr, nullptr);
        glCompileShader(shaderID);

        GLint success = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        GLint logLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1) {
            std::vector<char> infoLog(logLength);
            glGetShaderInfoLog(shaderID, logLength, nullptr, infoLog.data());
            std::cerr << "Shader compile log (" << shaderName << "):\n" << infoLog.data() << '\n';
        }

        if (success == GL_FALSE) {
            std::cerr << "Failed to compile shader: " << shaderName << '\n';
            return false;
        }
        return true;
    };

    // Create shader objects
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint geometryShaderID = 0;
    if (!geometryShaderCodeOpt.empty()) {
        geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
    }

    if (!compileShader(vertexShaderID, vertexShaderCodeOpt, vertexFilePath)) {
        glDeleteShader(vertexShaderID);
        if (geometryShaderID) glDeleteShader(geometryShaderID);
        glDeleteShader(fragmentShaderID);
        return 0;
    }

    if (!compileShader(fragmentShaderID, fragmentShaderCodeOpt, fragmentFilePath)) {
        glDeleteShader(vertexShaderID);
        if (geometryShaderID) glDeleteShader(geometryShaderID);
        glDeleteShader(fragmentShaderID);
        return 0;
    }

    if (geometryShaderID && !compileShader(geometryShaderID, geometryShaderCodeOpt, geometryFilePath)) {
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        glDeleteShader(geometryShaderID);
        return 0;
    }

    // Create program and link
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    if (geometryShaderID) {
        glAttachShader(programID, geometryShaderID);
    }

    glLinkProgram(programID);

    GLint linkSuccess = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &linkSuccess);

    GLint logLength = 0;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        std::vector<char> infoLog(logLength);
        glGetProgramInfoLog(programID, logLength, nullptr, infoLog.data());
        std::cerr << "Program link log:\n" << infoLog.data() << '\n';
    }

    if (linkSuccess == GL_FALSE) {
        std::cerr << "Failed to link shader program\n";
        glDeleteProgram(programID);
        programID = 0;
    }

    // Cleanup shaders after linking
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    if (geometryShaderID) {
        glDetachShader(programID, geometryShaderID);
        glDeleteShader(geometryShaderID);
    }

    return programID;
}
