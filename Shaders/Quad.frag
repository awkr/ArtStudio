#version 410 core

out vec4 fragColor;

in vec2 uv;
uniform sampler2D tex;

void main() {
    fragColor = texture(tex, uv);
    // fragColor = vec4(1, 0, 0, 1);
}
