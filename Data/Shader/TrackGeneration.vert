#version 330 core
layout(location = 0) in vec2 pos;

uniform vec2 canvasSize;
uniform vec2 contentPos;
uniform float contentZoom;

void main(){
    vec2 finalPos = ((pos + contentPos) / canvasSize - 1.0) * contentZoom;
    gl_Position =  vec4(finalPos.x, -finalPos.y, 0.0, 1);
    //pos = gl_VertexID % 2 == 0 ? 1.0 : 0.0;
    gl_PointSize = 10.0;
}