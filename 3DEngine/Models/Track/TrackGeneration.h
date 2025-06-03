#pragma once

#include "../../GUI/GUI.h"
#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <chrono>
#include <ranges>
#include <glm/glm.hpp>
#include <GL/glew.h>

using glm::vec2;
using glm::vec3;

class TrackGeneration : public GUIElement {
public:
    explicit TrackGeneration();

    void click(const vec2& mousePos, int action) override;
    void preDraw(const vec2& parentSize) override;
    void draw() override;

private:
    static constexpr float BALL_RADIUS = 20.0f;
    static constexpr int ROUGH_POINTS_AMOUNT = 10;


    std::array<GLuint, 3> vertexBuffers{};
    GLuint debugVertexBuffer{};

    std::array<std::vector<vec2>, 3> trackPoints;
    std::vector<vec2> interpolationPoints;

    bool movePoint = false;
    size_t pointIdx = 0;

    void generateRoughPath(std::array<std::vector<vec2>, 3>& pathPoints, bool randomize = false);
    void refinePath(std::vector<vec2>& pathPoints, int dir);
    vec2 getBallCenter(vec2 p1, vec2 p2, int dir);
    void createTrack(const std::array<std::vector<vec2>, 3>& pathPoints);

    float randomFloat();
};
