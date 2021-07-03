#version 410 core

uniform sampler2DRect depthBlenderTex; // 0
uniform sampler2DRect frontBlenderTex; // 1
uniform sampler2DRect backBlenderTex;  // 2

layout(location = 0) out vec4 fragColor;

void main() {
	// get the front and back blender colors
	vec4 frontColor = texture(frontBlenderTex, gl_FragCoord.xy);
	vec3 backColor = texture(backBlenderTex, gl_FragCoord.xy).rgb;

	// front + back
	// composite the front and back blending results
	fragColor.rgb = frontColor.rgb + backColor * frontColor.a;
}
