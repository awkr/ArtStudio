#version 410 core

out vec4 fragColor;

in vec2 uv; // interpolated 2D texture coordinate
uniform sampler2D tex; // texture map to display

void main() {
    fragColor = texture(tex, uv);
}
