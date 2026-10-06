#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inCol;

layout(push_constant) uniform PC {
    vec4 screen;   // x = width, y = height in pixels
} pc;

layout(location = 0) out vec2 vUV;
layout(location = 1) out vec4 vCol;

void main() {
    vec2 ndc = vec2(inPos.x / pc.screen.x * 2.0 - 1.0,
                    1.0 - inPos.y / pc.screen.y * 2.0);
    gl_Position = vec4(ndc, 0.0, 1.0);
    vUV = inUV;
    vCol = inCol;
}
