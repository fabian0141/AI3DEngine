#include "ObjModel.h"
#include "../../Camera.h"
#include "../../Loader/ShaderLoader.h"
#include <cstdio>


ObjModel::ObjModel(vector<vec3> vertices, vector<unsigned int> indices, vector<vec2> uvs, vector<vec3> normals)
    : vertices(move(vertices)), indices(move(indices)), uvs(move(uvs)), normals(move(normals)) 
{
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Vertex buffer
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Normal buffer
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // UV buffer if exists
    if (!uvs.empty()) {
        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    // Element buffer
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind VAO
}

ObjModel::~ObjModel() {
    if (vertexbuffer) glDeleteBuffers(1, &vertexbuffer);
    if (normalbuffer) glDeleteBuffers(1, &normalbuffer);
    if (uvbuffer) glDeleteBuffers(1, &uvbuffer);
    if (elementbuffer) glDeleteBuffers(1, &elementbuffer);
    if (vertexArrayID) glDeleteVertexArrays(1, &vertexArrayID);
}

void ObjModel::draw() {
    glBindVertexArray(vertexArrayID);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unique_ptr<ObjModel> ObjModel::getSubModel(vector<unsigned int> subIndices) {
    GLuint subElementBuffer;
    glGenBuffers(1, &subElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, subIndices.size() * sizeof(unsigned int), subIndices.data(), GL_STATIC_DRAW);

    unique_ptr<ObjModel> subModel = unique_ptr<ObjModel>(new ObjModel());

    subModel->vertices = vertices;
    subModel->indices = move(subIndices);
    subModel->uvs = uvs;
    subModel->normals = normals;

    subModel->vertexbuffer = vertexbuffer;
    subModel->normalbuffer = normalbuffer;
    subModel->uvbuffer = uvbuffer;
    subModel->elementbuffer = subElementBuffer;
    subModel->vertexArrayID = vertexArrayID;

    subModel->isSubModel = true;

    return subModel;
}
