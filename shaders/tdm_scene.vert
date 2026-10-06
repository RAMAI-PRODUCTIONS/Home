#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inCol;

layout(push_constant) uniform PC {
    mat4 mvp;
    vec4 fog;      // x = start, y = end
    vec4 fogCol;
} pc;

layout(location = 0) out vec4 vCol;
layout(location = 1) out vec4 vFog;

void main() {
    gl_Position = pc.mvp * vec4(inPos, 1.0);
    vCol = inCol;
    float d = gl_Position.w;
    float f = clamp((d - pc.fog.x) / max(pc.fog.y - pc.fog.x, 1.0), 0.0, 1.0);
    vFog = vec4(f, pc.fogCol.rgb);
}
