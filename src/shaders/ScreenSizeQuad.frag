#version 410 core

in vec2 uv;

uniform sampler2D tex;

out vec4 fragColor;

void main() {
    vec4 color = vec4(0);

    // determine the inverse of texture size
    vec2 delta = 1.0 / textureSize(tex, 0);

    // loop through the neighborhood
    for (int j = -3; j <= 3; ++j) {
        for (int i = -3; i <= 3; ++i) {
            // sum all samples in the neighborhood
            color += texture(tex, uv + (vec2(i, j) * delta));
        }
    }

    // divide by the total number of samples
    color /= 49.0;

    // return the average color
    fragColor = color;
}
