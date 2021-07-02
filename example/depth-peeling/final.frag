#version 410 core

layout(location = 0) out vec4 fragColor;

uniform sampler2DRect colorTex;	// color texture from previous pass
uniform vec4 backgroundColor;

void main() {
	vec4 color = texture(colorTex, gl_FragCoord.xy);
	fragColor = color + backgroundColor * color.a;
}
