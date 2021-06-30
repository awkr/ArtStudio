#version 410 core

layout(location = 0) in vec3 vertex;

uniform mat4 M[4];

void main() {
    gl_Position = M[gl_InstanceID] * vec4(vertex, 1);
}
