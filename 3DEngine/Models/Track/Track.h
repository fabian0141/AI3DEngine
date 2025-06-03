#pragma once

#include "../Model.h"
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Camera;

class Track : public Model {
public:
    Track(Camera& camera, GLuint programID);
    void draw() override;

private:
    static constexpr float BALL_RADIUS = 0.2f;
    static constexpr int PATH_COUNT = 3;

    Camera& camera;
    GLuint programID;
    std::array<GLuint, PATH_COUNT> vertexBuffers{};
    GLuint debugVertexBuffer{0};

    std::array<std::vector<glm::vec3>, PATH_COUNT> trackPoints;
    std::vector<glm::vec3> debugPoints;

    void generateRoughPath(std::array<std::vector<glm::vec2>, PATH_COUNT>& pathPoints);
    void refinePath(std::vector<glm::vec2>& pathPoints, int dir);
    glm::vec2 getBallCenter(const glm::vec2& p1, const glm::vec2& p2, int dir) const;
    void createTrack(const std::array<std::vector<glm::vec2>, PATH_COUNT>& pathPoints);
};
