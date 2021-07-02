#version 410 core

layout(location = 0) in vec3 vertex; //object space vertex position

uniform mat4 MVP;  //combined modelview projection matrix

void main()
{
	//get the clipspace vertex position
	gl_Position = MVP*vec4(vertex.xyz, 1);
}
