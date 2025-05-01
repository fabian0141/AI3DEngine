#include <vector>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;


class Spline {
    public:
        Spline(vector<vec2> cps, vector<float> knots);
        Spline(vector<vec2> points, bool circular = false);

        void getLine(vector<vec2>& linePoints);
        void createParallelLine(vector<vec2>& paraLine, float distance);
        void changeCurve(int idx, float rotate, float distance);
        void addCurve(float p, float rotate, float distance);

        vector<vec2> getInterpolationPoints();
    private:
        const int SEGMENTS_PER_CONTROLLPOINTS = 20;
        const float PARALLEL_DISTANCE = 0.3;

        vector<vec2> interpolationPoints;
        vector<vec2> conPoints;
        vector<vec2> tempPoints;
        vector<float> knots;
        vector<float> tempKnots;

        vec2 deBoor(float t, int column, vector<vec2>& returnVecs);
        float knotIndex(float t);
        void insertKnot(float t);
        void setEquidistantKnots(vector<vec2> points);

        void interpolate(vector<vec2> points);
        void interpolateCircular(vector<vec2> points);
        void solveEquation(vector<float> as, vector<float> bs, vector<float> cs, vector<vec2> res);
        void solveAlmostEquation(vector<float> as, vector<float> bs, vector<float> cs, vector<vec2> res);
};