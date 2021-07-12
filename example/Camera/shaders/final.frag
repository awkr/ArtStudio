#version 410 core

layout(location = 0) out vec4 fragColor;

uniform sampler2DRect colorTex;	// color texture from previous pass
uniform vec4 backgroundColor;

void main() {
	vec4 color = texture(colorTex, gl_FragCoord.xy);
	// combine the color read from the color texture with the background color
	// by multiplying the color alpha with the background color and adding the
	// product to the given color uniform
	fragColor = color + backgroundColor * color.a;
}
