#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 256) out;

uniform int subdivision;
uniform mat4 MVP;

void main() {
    vec4 v0 = gl_in[0].gl_Position;
    vec4 v1 = gl_in[1].gl_Position;
    vec4 v2 = gl_in[2].gl_Position;

    // determine the size of each subdivision
    float dx = abs(v0.x - v2.x) / subdivision;
    float dz = abs(v0.z - v1.z) / subdivision;

    float x = v0.x;
    float z = v0.z;

    // loop through all subdivisions and emit vertices after multiplying the object
    // space vertex positions with combined model view projection matrices.
    // move first in x axis, once reach the edge, reset x to the initiall x value
    // while incrementing the z value
    for (int i = 0; i < subdivision * subdivision; ++i) {
        gl_Position = MVP * vec4(x, 0, z, 1); EmitVertex();
        gl_Position = MVP * vec4(x, 0, z + dz, 1); EmitVertex();
        gl_Position = MVP * vec4(x + dx, 0, z, 1); EmitVertex();
        gl_Position = MVP * vec4(x + dx, 0, z + dz, 1); EmitVertex();
        EndPrimitive();
        x += dx;
        if ((i + 1) % subdivision == 0) {
            x = v0.x;
            z += dz;
        }
    }
}
