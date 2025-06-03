#pragma once

#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Spline {
public:
    explicit Spline(const std::vector<glm::vec2> cps, const std::vector<float> knots);
    explicit Spline(const std::vector<glm::vec2> points, bool circular = false);

    void getLine(std::vector<glm::vec2>& linePoints);
    void createParallelLine(std::vector<glm::vec2>& paraLine, float distance);
    void changeCurve(int idx, float rotate, float distance);
    void addCurve(float p, float rotate, float distance);

    vector<vec2> getInterpolationPoints();

private:
    static constexpr int SEGMENTS_PER_CONTROL_POINTS = 20;
    static constexpr float PARALLEL_DISTANCE = 0.3f;

    std::vector<glm::vec2> interpolationPoints;
    std::vector<glm::vec2> conPoints;
    std::vector<float> knots;

    void deBoor(float t, int column, std::vector<vec2>& returnVecs);
    int knotIndex(float t);
    void insertKnot(float t);

    void setEquidistantKnots(const std::vector<glm::vec2>& points);

    void interpolate(const std::vector<glm::vec2>& points);
    void interpolateCircular(const std::vector<glm::vec2>& points);
    void solveEquation(const std::vector<float>& as, const std::vector<float>& bs, const std::vector<float>& cs, std::vector<glm::vec2>& res);
    void solveAlmostEquation(const std::vector<float>& as, const std::vector<float>& bs, const std::vector<float>& cs, const std::vector<glm::vec2>& res);
};
