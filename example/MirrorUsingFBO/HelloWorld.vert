#version 410 core

layout(location = 0) in vec3 vertex; // object space vertex position
layout(location = 1) in vec3 color;

uniform mat4 MVP;

out vec4 smoothColor;

void main() {
    smoothColor = vec4(color, 1);
    gl_Position = MVP * vec4(vertex, 1);
}
