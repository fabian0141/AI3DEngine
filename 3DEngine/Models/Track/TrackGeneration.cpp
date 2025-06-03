#include "TrackGeneration.h"
#include "../../Camera.h"
#include "Spline.h"

TrackGeneration::TrackGeneration() : GUIElement(0) {
    
    std::random_device rd;
    std::mt19937 gen(rd());

    interpolationPoints = {
        {0, 200}, {0, 800}, {200, 1000}, {1200, 1000},
        {1400, 800}, {1400, 200}, {1200, 0}, {200, 0}
    };

    std::array<std::vector<vec2>, 3> pathPoints;
    generateRoughPath(pathPoints, true);
    refinePath(pathPoints[1], 1);
    refinePath(pathPoints[2], 1);
    createTrack(pathPoints);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    for (size_t i = 0; i < vertexBuffers.size(); ++i) {
        glGenBuffers(1, &vertexBuffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, trackPoints[i].size() * sizeof(vec3), trackPoints[i].data(), GL_STATIC_DRAW);
    }

    glGenBuffers(1, &debugVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, debugVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, interpolationPoints.size() * sizeof(vec3), interpolationPoints.data(), GL_STATIC_DRAW);
}

float TrackGeneration::randomFloat() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen);
}

void TrackGeneration::click(const vec2& mousePos, int action) {
    if (action == 1) {
        GUI &gui = GUI::Get();
        const auto scale = (1 + gui.contentZoom) / 2.0f;
        vec2 mPos = (mousePos - gui.canvasSize * (1 - gui.contentZoom)) / gui.contentZoom
                    - vec2(gui.contentPos.x, gui.contentPos.y);

        for (size_t i = 0; i < interpolationPoints.size(); ++i) {
            if (glm::distance(mPos, interpolationPoints[i]) < 20) {
                movePoint = true;
                pointIdx = i;
                break;
            }
        }
    } else {
        movePoint = false;
    }
}

void TrackGeneration::preDraw(const vec2& parentSize) {}

void TrackGeneration::draw() {
    GUI &gui = GUI::Get();

    if (movePoint) {
        vec2 mPos = (gui.mousePos - gui.canvasSize * (1 - gui.contentZoom)) / gui.contentZoom
                    - vec2(gui.contentPos.x, gui.contentPos.y);
        interpolationPoints[pointIdx] = mPos;

        std::array<std::vector<vec2>, 3> pathPoints;
        generateRoughPath(pathPoints, true);
        refinePath(pathPoints[1], 1);
        refinePath(pathPoints[2], 1);
        createTrack(pathPoints);

        for (size_t i = 0; i < vertexBuffers.size(); ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
            glBufferData(GL_ARRAY_BUFFER, trackPoints[i].size() * sizeof(vec3), trackPoints[i].data(), GL_STATIC_DRAW);
        }

        glBindBuffer(GL_ARRAY_BUFFER, debugVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, interpolationPoints.size() * sizeof(vec3), interpolationPoints.data(), GL_STATIC_DRAW);
    }

    const int programID = gui.getProgramID(GUIProgram::TRACK_GENERATION);
    glUseProgram(programID);
    glUniform2f(glGetUniformLocation(programID, "canvasSize"), gui.canvasSize.x, gui.canvasSize.y);
    glUniform2f(glGetUniformLocation(programID, "contentPos"), gui.contentPos.x, gui.contentPos.y);
    glUniform1f(glGetUniformLocation(programID, "contentZoom"), gui.contentZoom);
    glEnable(GL_PROGRAM_POINT_SIZE);

    for (size_t i = 0; i < vertexBuffers.size(); ++i) {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays((i == 1) ? GL_POINTS : GL_LINE_STRIP, 0, static_cast<GLsizei>(trackPoints[i].size()));
        if (i == 1) glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(trackPoints[i].size()));
        glDisableVertexAttribArray(0);
    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, debugVertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(interpolationPoints.size()));
    glDisableVertexAttribArray(0);
}

void TrackGeneration::generateRoughPath(std::array<std::vector<vec2>, 3>& pathPoints, bool randomize) {
    Spline spl(interpolationPoints, true);
    if (randomize) {
        for (int i = 0; i < 16; ++i) {
            spl.changeCurve(i, randomFloat() * 60 - 30, (randomFloat() - 0.5f) * 500.0f);
        }
    }

    spl.getLine(pathPoints[0]);
    spl.createParallelLine(pathPoints[1], -50);
    spl.createParallelLine(pathPoints[2], 50);

    interpolationPoints = spl.getInterpolationPoints();
}

void TrackGeneration::refinePath(std::vector<vec2>& pathPoints, int dir) {
    size_t i = 0;
    while (i < pathPoints.size()) {
        size_t k = (i + 1) % pathPoints.size();
        bool erased = false;

        for (int j = 1; j < 50; ++j) {
            size_t idx = (k + j) % pathPoints.size();
            if (glm::distance(pathPoints[i], pathPoints[k]) > glm::distance(pathPoints[i], pathPoints[idx])) {
                pathPoints.erase(pathPoints.begin() + k);
                erased = true;
                if (k <= i && i > 0) --i;
                break;
            }
        }
        if (!erased) ++i;
        if (pathPoints.size() <= 2) break;
    }
}

vec2 TrackGeneration::getBallCenter(vec2 p1, vec2 p2, int dir) {
    vec2 mid = 0.5f * (p1 + p2);
    vec2 dirVec = mid - p1;
    float dist = glm::dot(dirVec, dirVec);
    vec2 normal = dirVec / glm::sqrt(dist);
    return mid + vec2(normal.y, -normal.x) * (glm::sqrt(BALL_RADIUS * BALL_RADIUS - dist) * static_cast<float>(dir));
}

void TrackGeneration::createTrack(const std::array<std::vector<vec2>, 3>& pathPoints) {
    for (size_t i = 0; i < 3; ++i) {
        trackPoints[i] = pathPoints[i];
        if (!trackPoints[i].empty()) {
            trackPoints[i].push_back(trackPoints[i].front());
        }
    }
}
