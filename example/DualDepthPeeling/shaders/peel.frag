#version 410 core

layout(location = 0) out vec4 fragColor0;	// output to target 0
layout(location = 1) out vec4 fragColor1;	// output to target 1
layout(location = 2) out vec4 fragColor2;	// output to target 2

uniform vec4 color; // solid colour of the cube
uniform sampler2DRect depthBlenderTex; // depth blending output
uniform sampler2DRect frontBlenderTex; // front blending output
uniform float alpha; // fragment alpha

#define MAX_DEPTH 1.0	// max depth value to clear the depth with

void main() {
	// get the current fragment depth
	float fragDepth = gl_FragCoord.z;
	// get the depth value from the depth blending output
	vec2 depthBlender = texture(depthBlenderTex, gl_FragCoord.xy).xy;
	// get the front blending output
	vec4 forwardTemp = texture(frontBlenderTex, gl_FragCoord.xy);

	// depths and 1.0-alphaMult always increase
	// so we can use pass-through by default with MAX blending
	fragColor0.xy = depthBlender;

	// Front colors always increase (DST += SRC*ALPHA_MULT)
	// so we can use pass-through by default with MAX blending
	fragColor1 = forwardTemp;

	// Because over blending makes color increase or decrease,
	// we cannot pass-through by default.
	// Each pass, only one fragment can a color greater than 0
	fragColor2 = vec4(0.0);

	float nearestDepth = -depthBlender.x;
	float farthestDepth = depthBlender.y;
	float alphaMultiplier = 1.0 - forwardTemp.w;

	if (fragDepth < nearestDepth || fragDepth > farthestDepth) {
		// skip this depth in the peeling algorithm
		fragColor0.xy = vec2(-MAX_DEPTH);
		return;
	}

	if (fragDepth > nearestDepth && fragDepth < farthestDepth) {
		// this fragment needs to be peeled again
		fragColor0.xy = vec2(-fragDepth, fragDepth);
		return;
	}

	// If we made it here, this fragment is on the peeled layer from last pass
	// therefore, we need to shade it, and make sure it is not peeled any farther
	fragColor0.xy = vec2(-MAX_DEPTH);

	//if the fragment depth is the nearest depth, we blend the colour
	//to the second attachment
	if (fragDepth == nearestDepth) {
		fragColor1.xyz += color.rgb * alpha * alphaMultiplier;
		fragColor1.w = 1.0 - alphaMultiplier * (1.0 - alpha);
	} else {
		//otherwise we write to the thrid attachment
		fragColor2 += vec4(color.rgb, alpha);
	}
}
