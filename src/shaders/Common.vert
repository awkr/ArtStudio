#version 410 core

in vec3 vertex;

uniform mat4 MVP;

void main() {
    // multiply the combined MVP matrix with the object space position to get the clip space position
    gl_Position = MVP * vec4(vertex.xyz, 1);
}
