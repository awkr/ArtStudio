#version 410 core

layout(location = 0) in vec3 vertex; // object-space vertex position

uniform mat4 MVP;  // combined model-view-projection matrix

void main() {
	gl_Position = MVP * vec4(vertex.xyz, 1); // get the clip-space vertex position
}
