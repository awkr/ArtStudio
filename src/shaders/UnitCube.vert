#version 410 core

in vec3 vertex;
uniform mat4 MVP;

out vec3 color;

void main() {
    gl_Position = MVP * vec4(vertex, 1);
    // get the color from the object space vertex position by offsetting the vertex position
    color = vertex + 0.5;
}
