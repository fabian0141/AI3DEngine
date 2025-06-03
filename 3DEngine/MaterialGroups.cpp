#include "MaterialGroups.h"
#include "Models/Model.h"
#include "Camera.h"

MaterialGroups::MaterialGroups(shared_ptr<Camera> camera, GLuint programID) {
    this->camera = camera;
    this->programID = programID;
}

void MaterialGroups::addMaterial(string materialName, unique_ptr<Material> mat) {
    if (auto matG = matGroups.find(materialName); matG == matGroups.end()) {
        matGroups[materialName] = MaterialGroup {.mat = move(mat)};
    }
}

void MaterialGroups::addModel(string materialName, unique_ptr<Model> model) {
    if (auto matG = matGroups.find(materialName); matG != matGroups.end()) {
        matGroups[materialName].models.push_back(move(model));
    } else {
        printf("Could not find Material for Model\n");
    }
}

void MaterialGroups::draw() {

    glUseProgram(programID);
    GLuint modelViewProjectionID = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(modelViewProjectionID, 1, GL_FALSE, camera->getModelViewProjection());
    GLuint modelID = glGetUniformLocation(programID, "M");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, camera->getModel());
    GLuint viewID = glGetUniformLocation(programID, "V");
    glUniformMatrix4fv(viewID, 1, GL_FALSE, camera->getView());
    GLuint projectionID = glGetUniformLocation(programID, "P");
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, camera->getProjection());

    for (const auto& [key, value] : matGroups) {

        if (value.models.size() == 0) {
            continue;
        } 
        const auto& models = value.models;

        GLuint diffuseColor = glGetUniformLocation(programID, "diffuseColor");
        glUniform3f(diffuseColor, value.mat->diffuseColor[0], value.mat->diffuseColor[1], value.mat->diffuseColor[2]);
        GLuint specularColor = glGetUniformLocation(programID, "specularColor");
        glUniform3f(specularColor, value.mat->specularColor[0], value.mat->specularColor[1], value.mat->specularColor[2]);
        
        GLuint specularExponent = glGetUniformLocation(programID, "specularExponent");
        glUniform1f(specularExponent, value.mat->specularExponent / 1000);
        GLuint dissolve = glGetUniformLocation(programID, "dissolve");
        glUniform1f(dissolve, value.mat->dissolve);       
        
        GLuint camPosition = glGetUniformLocation(programID, "camPosition");
        vec3 camPos = camera->getPosition();
        glUniform3f(camPosition, camPos[0], camPos[1], camPos[2]);
        
        for_each(models.begin(), models.end(), [](const unique_ptr<Model> &model) { model->draw(); });
    }
}
