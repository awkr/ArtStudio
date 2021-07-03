#version 410 core

uniform sampler2DRect tempTex; //intermediate blending result

layout(location = 0) out vec4 fragColor;

void main() {
	// return the intermediate blending result
	fragColor = texture(tempTex, gl_FragCoord.xy);
	if (fragColor.a == 0) { discard; }
}
