#include "TrackGeneration.h"
#include <stdlib.h>
#include <GL/glew.h>
#include "../../Camera.h"
#include "Spline.h"
#include <cstdio>
#include <time.h>
#include <algorithm>  

float randomFloat()
{
    return (float)(rand()) / (float)(RAND_MAX);
}
 

TrackGeneration::TrackGeneration(Camera* camera, GLuint programID) {
    this->camera = camera;
    this->programID = programID;
    srand(time(NULL));

    vector<vec2> pathPoints[3];
    vector<vec2> leftPathPoints;
    vector<vec2> rightPathPoints;

    generateRoughPath(pathPoints);
    refinePath(pathPoints[1], -1);
    refinePath(pathPoints[2], -1);
    createTrack(pathPoints);

    //path

    GLuint VertexArrayID;
    for (int i = 0; i < 3; i++)
    {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        glGenBuffers(1, &vertexbuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, trackPoints[i].size() * sizeof(vec3), &trackPoints[i][0], GL_STATIC_DRAW);
    }

    //interpolation points
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &debugVertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, debugVertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, debugPoints.size() * sizeof(vec3), &debugPoints[0], GL_STATIC_DRAW);
}

void TrackGeneration::draw() {

    glUseProgram(programID);

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, camera->getModelViewProjection());

    for (int i = 0; i < 3; i++)
    {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
        glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_LINE_STRIP, 0, trackPoints[i].size()); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares

        glDisableVertexAttribArray(0);
    }
    


    glEnable(GL_PROGRAM_POINT_SIZE);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, debugVertexbuffer);
    glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_POINTS, 0, debugPoints.size()); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares

    glDisableVertexAttribArray(0);
}

void TrackGeneration::generateRoughPath(vector<vec2> pathPoints[3]) {

    vector<vec2> pts = {vec2( -2, -1), vec2(-2, 1), vec2( -1, 2), vec2( 1, 2), vec2( 2, 1), vec2(2, -1), vec2(1, -2), vec2(-1, -2)};
    Spline spl(pts, true);

    /*int amountCurves = rand() % 5 + 5;
    for (int i = 0; i < amountCurves; i++)
    {
        spl.addCurve(randomFloat(), randomFloat() * 60 - 30, (randomFloat() - 0.5) * 3);
    }*/

    //spl.addCurve(randomFloat(), randomFloat() * 60 - 30, (randomFloat() - 0.5) * 3);

    for (int i = 0; i < 16; i++)
    {
        spl.changeCurve(i, randomFloat() * 60 - 30, (randomFloat() - 0.5) * 4);
    }

    //spl.addCurve(0.3, 90, -0.5);
    spl.getLine(pathPoints[0]);
    spl.createParallelLine(pathPoints[1], -0.2);
    spl.createParallelLine(pathPoints[2], 0.2);

    vector<vec2> interpolationPoints = spl.getDebugPoints();
    for (int i = 0; i < interpolationPoints.size(); i++)
    {
        debugPoints.push_back(vec3(interpolationPoints[i].x, interpolationPoints[i].y, 0.f));
    }
}

void TrackGeneration::refinePath(vector<vec2> &pathPoints, int dir) {
    int i = 0;
    while (i < pathPoints.size())
    {
        int k = i + 1;
        vec2 middle = getBallCenter(pathPoints[i], pathPoints[k], dir);
        
        for (int j = 0; j < 10; j++)
        {
            if (distance(middle, pathPoints[k+j]) < BALL_RADIUS) {
                pathPoints.erase(pathPoints.begin() + k);
                break;
            }
        }
        i++;
    }    
}

vec2 TrackGeneration::getBallCenter(vec2 p1, vec2 p2, int dir) {
    vec2 middelPoint = 0.5f * (p1 + p2);
    vec2 dirVec = middelPoint - p1;
    float dist = dot(dirVec, dirVec);
    vec2 normal = dirVec / sqrt(dist);

    return middelPoint + vec2(normal.y, -normal.x) * (sqrt(BALL_RADIUS * BALL_RADIUS - dist) * dir);
}


void TrackGeneration::createTrack(vector<vec2> pathPoints[3]) {
    for (int i = 0; i < pathPoints[0].size(); i++)
    {
        trackPoints[0].push_back(vec3(pathPoints[0][i].x, pathPoints[0][i].y, 0.f));
        trackPoints[1].push_back(vec3(pathPoints[1][i].x, pathPoints[1][i].y, 0.f));
        trackPoints[2].push_back(vec3(pathPoints[2][i].x, pathPoints[2][i].y, 0.f));
    }
}