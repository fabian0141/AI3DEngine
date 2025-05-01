#include "Spline.h"
#include <glm/gtx/rotate_vector.hpp>
#include <cstdio>

Spline::Spline(vector<vec2> cps, vector<float> knots) {
    conPoints = cps;
    this->knots = knots;
}

Spline::Spline(vector<vec2> points, bool circular) {

    if (!circular) {
        knots.push_back(0);
        knots.push_back(0);
        knots.push_back(0);
        knots.push_back(0);

        for (int i = 0; i < points.size() - 1; i++)
        {
            knots.push_back(knots.back() + distance(points[i+1], points[i]));
        }

        knots.push_back(knots.back());
        knots.push_back(knots.back());
        knots.push_back(knots.back());
        interpolate(points);
    } else {
        for (int i = 0; i < points.size() + 7; i++)
        {
            knots.push_back(i);
        }
        interpolateCircular(points);
    }
}

void Spline::getLine(vector<vec2>& linePoints) {

    int totalSegments = SEGMENTS_PER_CONTROLLPOINTS * conPoints.size();
    float segmentLength = (knots[knots.size() - 4] - knots[3]) / totalSegments;

    for (int i = 0; i < totalSegments; i++)
    {
        vector<vec2> linePoint;
        deBoor(knots[3] + i * segmentLength, 4, linePoint);
        linePoints.push_back(linePoint[0]);
    }
    /*vector<vec2> linePoint;
    deBoor(knots[knots.size() - 4], 4, linePoint);
    linePoints.push_back(linePoint[0]);*/
}

void Spline::createParallelLine(vector<vec2>& paraLine, float distance) {

    int totalSegments = SEGMENTS_PER_CONTROLLPOINTS * conPoints.size();
    float segmentLength = (knots[knots.size() - 4] - knots[3]) / totalSegments;

    for (int i = 0; i < totalSegments; i++)
    {
        vector<vec2> tangentPoint;
        deBoor(knots[3] + i * segmentLength, 3, tangentPoint);

        vec2 tangent = tangentPoint[1] - tangentPoint[0];
        vec2 normal = normalize(vec2(-tangent.y, tangent.x)) * distance;

        vector<vec2> linePoint;
        deBoor(knots[3] + i * segmentLength, 4, linePoint);
        paraLine.push_back(linePoint[0] + normal);
    }

    /*vector<vec2> tangentPoint;
    deBoor(knots[knots.size() - 4], 3, tangentPoint);

    vec2 tangent = tangentPoint[1] - tangentPoint[0];
    vec2 normal = normalize(vec2(-tangent.y, tangent.x)) * distance;

    vector<vec2> linePoint;
    deBoor(knots[knots.size() - 4], 4, linePoint);
    paraLine.push_back(linePoint[0] + normal);*/
}

void Spline::changeCurve(int idx, float rot, float dist) {
    idx += 3;
    float t = knots[idx];
    vector<vec2> tangentPoint;
    deBoor(t, 3, tangentPoint);

    vec2 tangent = tangentPoint[1] - tangentPoint[0];
    vec2 dir = rotate(normalize(tangent), rot) * dist;

    vector<vec2> insertPoint;
    deBoor(t, 4, insertPoint);

    vector<vec2> points;

    for (int i = 3; i < knots.size() - 4; i++)
    {
        if (i == idx) {
            points.push_back(insertPoint[0] + dir);
        } else {
            vector<vec2> linePoint;
            deBoor(knots[i], 4, linePoint);
            points.push_back(linePoint[0]);
        }
    }

    for (int i = 0; i < points.size(); i++)
    {
        printf("(%f, %f)\n", points[i].x, points[i].y);
    }
    

    interpolateCircular(points);
}

void Spline::addCurve(float p, float rot, float dist) {
    float t = p * (knots[knots.size() - 4] - knots[3]) + knots[3];
    vector<vec2> tangentPoint;
    deBoor(t, 3, tangentPoint);

    vec2 tangent = tangentPoint[1] - tangentPoint[0];
    vec2 dir = rotate(normalize(tangent), rot) * dist;

    vector<vec2> insertPoint;
    deBoor(t, 4, insertPoint);

    vector<vec2> points;
    bool inserted = false;

    for (int i = 3; i < knots.size() - 4; i++)
    {
        if (!inserted && knots[i] > t) {
            inserted = true;
            points.push_back(insertPoint[0] + dir);
        }

        vector<vec2> linePoint;
        deBoor(knots[i], 4, linePoint);
        points.push_back(linePoint[0]);
    }

    for (int i = 0; i < points.size(); i++)
    {
        printf("(%f, %f)\n", points[i].x, points[i].y);
    }
    

    setEquidistantKnots(points);
    interpolateCircular(points);
}

vector<vec2> Spline::getInterpolationPoints() {
    return interpolationPoints;
}



vec2 Spline::deBoor(float t, int column, vector<vec2>& returnVecs) {
    if (t == knots[knots.size() - 4]) {
        t -= 0.000001f;
    }

    int i = knotIndex(t) - 3;
    vector<vec2> prev(conPoints.begin() + i, conPoints.begin() + (i + 4));

    for (int k = 1; k < column; k++)
    {
        returnVecs.clear();
        for (int j = 0; j < 4 - k; j++)
        {
            float a = (t - knots[i+j+k]) / (knots[i+j+4] - knots[i+j+k]);
            returnVecs.push_back(vec2((1-a) * prev[j] + a * prev[j+1]));
        }
        prev = returnVecs;
    }
}

float Spline::knotIndex(float t) {

    int middle = knots.size() / 2;
    int lower = 0;
    int upper = knots.size();

    while (upper - lower > 1) {
        if (t < knots[middle]) {
            upper = middle;
        } else {
            lower = middle;
        }
        middle = (lower + upper) / 2;
    }
    return lower;
}

void Spline::insertKnot(float t) {

}

void Spline::setEquidistantKnots(vector<vec2> points) {
    knots.clear();
    for (int i = 0; i < points.size() + 7; i++)
    {
        knots.push_back(i);
    }
}

void Spline::interpolate(vector<vec2> points) {

    int n = points.size() + 2;
    vector<float> alphas(n);
    vector<float> betas(n);
    vector<float> gammas(n);


    for (int i = 2; i < n - 2; i++)
    {
        alphas[i] = (knots[i+2] - knots[i]) / (knots[i+3] - knots[i]);
        betas[i] = (knots[i+2] - knots[i+1]) / (knots[i+3] - knots[i+1]);
        gammas[i] = (knots[i+2] - knots[i+1]) / (knots[i+4] - knots[i+1]);
    }
    
    vector<float> as(n);
    vector<float> bs(n);
    vector<float> cs(n);

    for (int i = 2; i < n - 2; i++)
    {
        as[i] = (1 - betas[i]) * (1 - alphas[i]);
        bs[i] = (1 - betas[i]) * alphas[i] + betas[i] * (1 - gammas[i]);
        cs[i] = betas[i] * gammas[i];
    }

    bs[0] = 1;
    cs[0] = 0;
    as[1] = -1;
    bs[1] = 1 + alphas[2];
    cs[1] = -alphas[2];

    as[n-2] = -1 + gammas[n-3];
    bs[n-2] = -gammas[n-3] + 2;
    cs[n-2] = -1;
    as[n-1] = 0;
    bs[n-1] = 1;
    
    vector<vec2> res;
    res.push_back(points[0]);
    res.push_back(vec2(0));
    res.insert(res.end(), points.begin() + 1, points.end() - 1);
    res.push_back(vec2(0));
    res.push_back(points.back());

    solveEquation(as, bs, cs, res);
}

void Spline::solveEquation(vector<float> as, vector<float> bs, vector<float> cs, vector<vec2> res) {

    vector<float> vs(res.size());
    vector<float> zs(res.size());
    vector<vec2> ys(res.size() + 1);

    vs[0] = 0; 
    as[0] = 0;
    cs[res.size() - 1] = 0;
    ys[0] = vec2(0);

    for (int i = 0; i < res.size(); i++) {
        zs[i] = 1 / (bs[i] - as[i] * vs[i]);
        vs[i+1] = zs[i] * cs[i];
        ys[i+1] = zs[i] * (res[i] - as[i] * ys[i]) ;

    }
    
    conPoints.resize(res.size());
    conPoints[res.size() - 1] = ys[res.size()];
    for (int i = res.size() - 2; i >= 0; i--) {
        conPoints[i] = ys[i+1] - vs[i+1] * conPoints[i+1];
    }
}

void Spline::interpolateCircular(vector<vec2> points) {

    interpolationPoints = points;

    int n = points.size();
    
    vector<float> as(n + 1);
    vector<float> bs(n + 1);
    vector<float> cs(n + 1);

    for (int i = 1; i < n + 1; i++)
    {
        as[i] = 1.0 / 6;
        bs[i] = 4.0 / 6;
        cs[i] = 1.0 / 6;
    }
    solveAlmostEquation(as, bs, cs, points);

    conPoints[n] = conPoints[0];
    conPoints[n+1] = conPoints[1];
    conPoints[n+2] = conPoints[2];
}

void Spline::solveAlmostEquation(vector<float> as, vector<float> bs, vector<float> cs, vector<vec2> res) {

    int n = res.size();

    vector<float> vs(n + 1);
    vector<float> zs(n + 1);
    vector<vec2> ys(n + 1);
    vector<float> ss(n + 1);
    vs[0] = 0;
    ys[0] = vec2(0);
    ss[0] = 1;

    for (int i = 1; i < n; i++) {
        zs[i] = 1 / (bs[i] + as[i] * vs[i-1]);
        vs[i] = -zs[i] * cs[i];
        ys[i] = zs[i] * (res[i-1] - as[i] * ys[i-1]) ;
        ss[i] = -as[i] * ss[i-1] * zs[i];
    }

    vector<float> ts(n + 1);
    vector<vec2> ws(n + 1);
    ts[n] = 1;
    ws[n] = vec2(0);

    for (int i = n - 1; i > 0; i--) {
        ts[i] = vs[i] * ts[i+1] + ss[i];
        ws[i] = vs[i] * ws[i+1] + ys[i];
    }
    
    conPoints.resize(n + 3);
    conPoints[n-1] = (res[n-1] - cs[n] * ws[1] - as[n] * ws[n-1]) / (cs[n] * ts[1] + as[n] * ts[n-1] + bs[n]);
    for (int i = n - 1; i > 0; i--) {
        conPoints[i-1] = ts[i] * conPoints[n-1] + ws[i];
    }
}

