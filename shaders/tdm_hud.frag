#version 450

layout(location = 0) in vec2 vUV;
layout(location = 1) in vec4 vCol;

layout(set = 0, binding = 0) uniform sampler2D uFont;
layout(location = 0) out vec4 outColor;

void main() {
    float mask = texture(uFont, vUV).r;
    outColor = vec4(vCol.rgb, vCol.a * mask);
}
