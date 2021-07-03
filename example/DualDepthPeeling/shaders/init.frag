#version 410 core

layout(location = 0) out vec4 fragColor;

void main() {
	// set the fragment colour as -fragment depth and fragment depth
	// in the red and green channel. This when combined with min/max
	// blending will help in peeling front and back layers simultaneously
	fragColor.xy = vec2(-gl_FragCoord.z, gl_FragCoord.z);
}
