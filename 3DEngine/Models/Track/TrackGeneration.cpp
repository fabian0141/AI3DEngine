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
 

TrackGeneration::TrackGeneration(GUI* gui) : GUIElement(0), gui(gui) {
    srand(time(NULL));

    vector<vec2> leftPathPoints;
    vector<vec2> rightPathPoints;

    interpolationPoints = {vec2( 0, 200), vec2(0, 800), vec2( 200, 1000), vec2( 1200, 1000), vec2( 1400, 800), vec2(1400, 200), vec2(1200, 0), vec2(200, 0)};

    vector<vec2> pathPoints[3];
    generateRoughPath(pathPoints, false);
    printf("%d ", pathPoints[1].size());
    refinePath(pathPoints[1], 1);
    printf("%d \n", pathPoints[1].size());
    refinePath(pathPoints[2], 1);
    createTrack(pathPoints);

    //path

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    for (int i = 0; i < 3; i++)
    {
        glGenBuffers(1, &vertexbuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, trackPoints[i].size() * sizeof(vec3), &trackPoints[i][0], GL_STATIC_DRAW);
    }

    //interpolation points
    glGenBuffers(1, &debugVertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, debugVertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, interpolationPoints.size() * sizeof(vec3), &interpolationPoints[0], GL_STATIC_DRAW);
}

void TrackGeneration::click(const vec2 &mousePos, int action) {

    //vec2 mPos = (mousePos / gui->canvasSize - 1.0f) * gui->contentZoom;
    if (action == 1) {
        float scale = (1 + gui->contentZoom) / 2.0f;
        vec2 mPos = (mousePos - gui->canvasSize * (1 - gui->contentZoom)) / gui->contentZoom - vec2(gui->contentPos.x, gui->contentPos.y);
        //printf("(%f, %f) (%f, %f) (%f, %f) %f     ", mousePos.x, mousePos.y, mPos.x, mPos.y, gui->contentPos.x, gui->contentPos.y, gui->contentZoom);

        for (size_t i = 0; i < interpolationPoints.size(); i++)
        {
            //printf("(%f, %f) ", interpolationPoints[i].x, interpolationPoints[i].y);

            if (distance(mPos, interpolationPoints[i]) < 20) {
                //printf("Point hit!\n");
                movePoint = true;
                pointIdx = i;
                break;
            }
        }
        //printf("\n");
    } else {
        movePoint = false;
    }
}

void TrackGeneration::preDraw() {}

void TrackGeneration::draw() {
    if (movePoint) {
        vec2 mPos = (gui->mousePos - gui->canvasSize * (1 - gui->contentZoom)) / gui->contentZoom - vec2(gui->contentPos.x, gui->contentPos.y);

        interpolationPoints[pointIdx] = mPos;
        vector<vec2> pathPoints[3];
        generateRoughPath(pathPoints, false);
        refinePath(pathPoints[1], 1);
        refinePath(pathPoints[2], 1);
        createTrack(pathPoints);

        for (int i = 0; i < 3; i++)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
            glBufferData(GL_ARRAY_BUFFER, trackPoints[i].size() * sizeof(vec3), &trackPoints[i][0], GL_STATIC_DRAW);
        }

        //interpolation points
        glBindBuffer(GL_ARRAY_BUFFER, debugVertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, interpolationPoints.size() * sizeof(vec3), &interpolationPoints[0], GL_STATIC_DRAW);
    }

    int programID = gui->getProgramID(GUI::TRACK_GENERATION_PROGRAM);
    glUseProgram(programID);
    GLuint canvasSizeID = glGetUniformLocation(programID, "canvasSize");
    glUniform2f(canvasSizeID, gui->canvasSize.x, gui->canvasSize.y);
    GLuint contenPosID = glGetUniformLocation(programID, "contentPos");
    glUniform2f(contenPosID, gui->contentPos.x, gui->contentPos.y);
    GLuint contentZoomID = glGetUniformLocation(programID, "contentZoom");
    glUniform1f(contentZoomID, gui->contentZoom);
    glEnable(GL_PROGRAM_POINT_SIZE);

    for (int i = 0; i < 3; i++)
    {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
        glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_LINE_STRIP, 0, trackPoints[i].size()); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares

        if (i == 1) {
            glDrawArrays(GL_POINTS, 0, trackPoints[i].size()); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
        }

        glDisableVertexAttribArray(0);
    }
    



    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, debugVertexbuffer);
    glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    2,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_POINTS, 0, interpolationPoints.size()); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares

    glDisableVertexAttribArray(0);
}

void TrackGeneration::generateRoughPath(vector<vec2> pathPoints[3], bool randomize) {

    Spline spl(interpolationPoints, true);
    if (randomize) {
        for (int i = 0; i < 16; i++)
        {
            spl.changeCurve(i, randomFloat() * 60 - 30, (randomFloat() - 0.5) * 4);
        }
    }

    //spl.addCurve(0.3, 90, -0.5);
    spl.getLine(pathPoints[0]);
    spl.createParallelLine(pathPoints[1], -50);
    spl.createParallelLine(pathPoints[2], 50);

    vector<vec2> interPoints = spl.getInterpolationPoints();
    interpolationPoints.clear();
    copy(interPoints.begin(), interPoints.end(), back_inserter(interpolationPoints));
}

void TrackGeneration::refinePath(vector<vec2> &pathPoints, int dir) {
    int i = 0;
    while (i < pathPoints.size())
    {
        //printf("(%f,%f) ", pathPoints[i].x, pathPoints[i].y);
        int k = (i + 1) % pathPoints.size();
        //float t;
        //vec2 middle = getBallCenter(pathPoints[i], pathPoints[k], dir);
        
        for (int j = 1; j < 50; j++)
        {
            //float dist = distance(middle, pathPoints[k+j]);
            //if (dist < BALL_RADIUS) {
            if (distance(pathPoints[i], pathPoints[k]) > distance(pathPoints[i], pathPoints[(k+j) % pathPoints.size()])) {

                if (k == pathPoints.size()-1) {
                    /*pathPoints[k] = pathPoints[1];
                    pathPoints.erase(pathPoints.begin());

                    i--;*/
                    pathPoints.erase(pathPoints.begin() + k);
                    i -= 2;
                    break;
                } else {
                    pathPoints.erase(pathPoints.begin() + k);
                    i--;
                    if (k < i) {
                        i--;
                    }
                    break;
                }
                //printf("(%d,%f,%f) ", k, distance(pathPoints[i], pathPoints[k]), distance(pathPoints[i], pathPoints[(k+j) % pathPoints.size()]));

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
    trackPoints[0].clear();
    trackPoints[1].clear();
    trackPoints[2].clear();

    copy(pathPoints[0].begin(), pathPoints[0].end(), back_inserter(trackPoints[0]));
    trackPoints[0].push_back(trackPoints[0][0]);
    copy(pathPoints[1].begin(), pathPoints[1].end(), back_inserter(trackPoints[1]));
    trackPoints[1].push_back(trackPoints[1][0]);
    copy(pathPoints[2].begin(), pathPoints[2].end(), back_inserter(trackPoints[2]));
    trackPoints[2].push_back(trackPoints[2][0]);
}