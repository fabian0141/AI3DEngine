#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;

//out float pos;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);
    //pos = gl_VertexID % 2 == 0 ? 1.0 : 0.0;
    gl_PointSize = 10.0;

}