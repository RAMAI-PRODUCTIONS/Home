#version 450

layout(location = 0) in vec4 vCol;
layout(location = 1) in vec4 vFog;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(mix(vCol.rgb, vFog.yzw, vFog.x), vCol.a);
}
