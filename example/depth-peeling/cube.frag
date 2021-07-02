#version 410 core

layout(location = 0) out vec4 fragColor; // output fragment color

uniform vec4 color;

void main() {
    fragColor = color;
}
