#version 330 core
out vec4 color;

uniform sampler2D font;

in vec2 uv;

void main(){
    vec3 col = texture(font, uv).xyz;
    color = vec4(col, col.x);
    //color = vec4(1.0);
}