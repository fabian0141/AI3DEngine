#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 vertexNormal_modelspace;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 camPosition;


out vec3 Normal_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 Position_worldspace;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);

    vec3 LightPosition_worldspace = vec3(150.0, 200.0, 100.0);
    Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;

    vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
    vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    Normal_cameraspace = vertexNormal_modelspace;
}