#include "Track.h"
#include "../../Camera.h"
#include "Spline.h"

#include <random>
#include <algorithm>
#include <cmath>
#include <ranges>

namespace {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

    float randomFloat() {
        return dist01(rng);
    }
}

Track::Track(Camera& cam, GLuint progID)
    : camera(cam), programID(progID)
{
    std::array<std::vector<vec2>, PATH_COUNT> pathPoints;
    generateRoughPath(pathPoints);

    refinePath(pathPoints[1], -1);
    refinePath(pathPoints[2], -1);

    createTrack(pathPoints);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        glGenBuffers(1, &vertexBuffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER,
                     trackPoints[i].size() * sizeof(vec3),
                     trackPoints[i].data(),
                     GL_STATIC_DRAW);
    }

    glGenBuffers(1, &debugVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, debugVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 debugPoints.size() * sizeof(vec3),
                 debugPoints.data(),
                 GL_STATIC_DRAW);
}

void Track::draw() {
    glUseProgram(programID);

    GLuint matrixID = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, camera.getModelViewProjection());

    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(trackPoints[i].size()));
        glDisableVertexAttribArray(0);
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, debugVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(debugPoints.size()));
    glDisableVertexAttribArray(0);
}

void Track::generateRoughPath(std::array<std::vector<vec2>, PATH_COUNT>& pathPoints) {
    std::vector<vec2> controlPoints = {
        {-2.f, -1.f}, {-2.f, 1.f}, {-1.f, 2.f}, {1.f, 2.f},
        {2.f, 1.f}, {2.f, -1.f}, {1.f, -2.f}, {-1.f, -2.f}
    };

    Spline spl(controlPoints, true);

    for (int i = 0; i < 16; ++i) {
        spl.changeCurve(i, randomFloat() * 60.f - 30.f, (randomFloat() - 0.5f) * 4.f);
    }

    spl.getLine(pathPoints[0]);
    spl.createParallelLine(pathPoints[1], -0.2f);
    spl.createParallelLine(pathPoints[2], 0.2f);

    /*for (const auto& p : spl.getDebugPoints()) {
        debugPoints.emplace_back(p.x, p.y, 0.f);
    }*/
}

void Track::refinePath(std::vector<vec2>& pathPoints, int dir) {
    std::size_t i = 0;
    while (i + 1 < pathPoints.size()) {
        std::size_t k = i + 1;
        const vec2 middle = getBallCenter(pathPoints[i], pathPoints[k], dir);

        bool erased = false;
        for (std::size_t j = 0; j + k < pathPoints.size() && j < 10; ++j) {
            if (glm::distance(middle, pathPoints[k + j]) < BALL_RADIUS) {
                pathPoints.erase(pathPoints.begin() + k);
                erased = true;
                break;
            }
        }

        if (!erased) {
            ++i;
        }
    }
}

vec2 Track::getBallCenter(const vec2& p1, const vec2& p2, int dir) const {
    const vec2 mid = 0.5f * (p1 + p2);
    const vec2 delta = mid - p1;
    const float d2 = glm::dot(delta, delta);

    if (d2 > BALL_RADIUS * BALL_RADIUS) return mid;  // Avoid sqrt of negative

    const vec2 normal = delta / std::sqrt(d2);
    float offset = std::sqrt(std::max(BALL_RADIUS * BALL_RADIUS - d2, 0.f));

    return mid + vec2(normal.y, -normal.x) * offset * static_cast<float>(dir);
}

void Track::createTrack(const std::array<std::vector<vec2>, PATH_COUNT>& pathPoints) {
    for (std::size_t i = 0; i < pathPoints[0].size(); ++i) {
        for (std::size_t j = 0; j < PATH_COUNT; ++j) {
            trackPoints[j].emplace_back(pathPoints[j][i].x, pathPoints[j][i].y, 0.f);
        }
    }
}
