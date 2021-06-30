#version 410 core

in vec2 vertex;

out vec2 uv;

void main() {
    gl_Position = vec4(vertex.xy * 2 - 1.0, 0, 1);
    uv = vertex;
}
