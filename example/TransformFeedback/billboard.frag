#version 410 core

uniform sampler2D tex;

in vec2 texCoord;
out vec4 fragCoord;

void main()
{
    fragCoord = texture(tex, texCoord);
    if (fragCoord.r >= 0.85 && fragCoord.g >= 0.85 && fragCoord.b >= 0.85) {
        discard;
    }

    // fragCoord = vec4(1, 0, 0, 1);
}
