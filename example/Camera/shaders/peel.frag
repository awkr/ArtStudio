#version 410 core

layout(location = 0) out vec4 fragColor;

uniform vec4 color;
uniform sampler2DRect  depthTex;

void main() {
	// read the depth value from the depth texture
	float frontDepth = texture(depthTex, gl_FragCoord.xy).r;

	// compare the current fragment depth with the depth in the depth texture
	// if it is less, discard the current fragment
	if(gl_FragCoord.z <= frontDepth)
		discard;

	// otherwise set the given color uniform as the final output
	fragColor = color;
}
