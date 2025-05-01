#version 330 core

uniform sampler2D fontTex;

layout(location = 0) out uvec4 color;
//out vec4 color;
void main(){
    for (int x = 63; x >= 0; x--) {
        for (int y = 0; y < 64; y++) {
            if (texture(fontTex, (((gl_FragCoord.xy - 0.5) * 64.0 + 0.5) + vec2(x,y)) / 1024.0).x > 0.0) {
                color = uvec4(int(gl_FragCoord.x) * 64 + x, 1, 0, 1);
                return;
            }
        }
    }
    color = uvec4(0, 1, 0, 1);
    //color = uvec4(texture(fontTex, gl_FragCoord.xy / 16.0).x * 100, 0, 0, 1);
}