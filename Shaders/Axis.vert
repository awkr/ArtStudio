#version 410 core

in vec3 vertex;

uniform mat4 MVP;

out vec4 axisColor;

void main() {
    gl_Position = MVP * vec4(vertex.xyz, 1);

    axisColor = vec4(0, 0, 0, 1);
    axisColor[gl_VertexID / 2] = 1;
}
