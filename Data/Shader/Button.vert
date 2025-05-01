#version 330 core
layout (location = 0) in vec2 pos;

uniform vec2 canvasSize;

void main(){
    vec2 finalPos = (pos / canvasSize) - 1.0;
    gl_Position = vec4(finalPos.x, -finalPos.y, 0.0, 1.0);
}