#include "ObjModel.h"
#include "../../Camera.h"
#include "../../Loader/ShaderLoader.h"
#include <cstdio>

using namespace std;

ObjModel::ObjModel(vector< vec3 >* vertices, vector< unsigned int >* indices, vector< vec2 >* uvs, vector< vec3 >* normals) {
    this->vertices = vertices;
    this->indices = indices;
    this->uvs = uvs;
    this->normals = normals;

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(vec3),  &(*vertices)[0], GL_STATIC_DRAW);

    //printf("%p, %p, %p\n", (void*)&vertices[0], (void*)&(*vertices)[0], (*vertices));

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &(*indices)[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(glm::vec3), &(*normals)[0], GL_STATIC_DRAW);

    printf("Vertices: %d %d %d\n", vertices->size(), indices->size(), normals->size());
}

ObjModel::ObjModel() {
    isSubModel = true;
}



ObjModel::~ObjModel() {
    if (!isSubModel) {
        delete vertices;
        delete uvs;
        delete normals;
    }
    delete indices;
}

void ObjModel::draw() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );
    // Draw the triangle !

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, (void*) 0); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(2);
}

ObjModel* ObjModel::getSubModel(vector< unsigned int >* subIndices) {
    GLuint subElementBuffer;
    glGenBuffers(1, &subElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, subIndices->size() * sizeof(unsigned int), (void*)&(*subIndices)[0], GL_STATIC_DRAW);


    ObjModel* subModel = new ObjModel();

    subModel->vertices = vertices;
    subModel->indices = subIndices;
    subModel->uvs = uvs;
    subModel->normals = normals;

    subModel->vertexbuffer = vertexbuffer;
    subModel->elementbuffer = subElementBuffer;
    subModel->normalbuffer = normalbuffer;
}
