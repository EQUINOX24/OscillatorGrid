#version 450 core

layout(location = 0) in vec2 attrPos;
layout(location = 1) in vec2 attrPos_0;

void main() {
    gl_Position = vec4(attrPos + attrPos_0, 0.0f, 1.0f);
}