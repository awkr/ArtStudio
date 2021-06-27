#version 410 core

layout(location = 0) in vec3 vertex;

uniform mat4 MVP;
uniform float time; // elapsed time

const float amplitude = 0.125;
const float frequency = 4;
const float PI = 3.141592654;

void main() {
    // get the Euclidean distane of the current vertex from the center of the mesh
    float distance = length(vertex);
    // create a sin function using the distance, multiply frequency and add the elapsed time
    float y = amplitude * sin(-PI * distance * frequency + time);
    // multiply the MVP matrices with the new position to get the clip space position
    gl_Position = MVP * vec4(vertex.x, y, vertex.z, 1);
}
