#version 410 core

in vec3 vertex;

uniform mat4 MVP;

out vec2 uv;

void main() {
    gl_Position = MVP * vec4(vertex.xyz, 1);
    uv = vertex.xy;
}
