#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 128) out;

uniform vec2 canvasSize;
uniform int textSize;
uniform int text[64];

uniform int fontMeta[256];

out vec2 uv;

void main() {    

    vec4 pos = gl_in[0].gl_Position;

    for (int i = 0; i < textSize; i++) {

        int width = 15;
        int uvLeft = text[i] % 16;
        int uvBottom = 15 - (text[i] / 16);
        if (text[i] > -1) {
            width = fontMeta[text[i]] - 64 * (text[i] % 16);

        } else {
            uvLeft = 1;
            uvBottom = 1;
        }



        gl_Position = pos + vec4(0.0, -64 / canvasSize.y, 0.0, 0.0);
        uv = vec2(uvLeft, uvBottom) / 16.0;
        //uv = vec2(0.0,0.0);
        EmitVertex();

        gl_Position = pos + vec4(width / canvasSize.x, -64 / canvasSize.y, 0.0, 0.0);
        uv = vec2(uvLeft + width/64.0, uvBottom) / 16.0;
        //uv = vec2(1.0,0.0);
        EmitVertex();

        gl_Position = pos;
        uv = vec2(uvLeft, uvBottom+1) / 16.0;
        //uv = vec2(0.0,1.0);
        EmitVertex();

        gl_Position = pos + vec4(width / canvasSize.x, 0.0, 0.0, 0.0);
        uv = vec2(uvLeft + width/64.0, uvBottom+1) / 16.0;
        //uv = vec2(1.0,1.0);
        EmitVertex();
        EndPrimitive();

        pos.x += (width+2) / canvasSize.x;
    }
}  