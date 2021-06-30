#version 410 core

in vec3 vertex;

uniform mat4 MVP;

out vec2 uv; // output texture coordinate to fragment shader

void main() {
    gl_Position = MVP * vec4(vertex.xyz, 1);
    uv = vec2(vertex.x + 1, vertex.y) * 0.5;
}
