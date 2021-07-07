#version 410 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 VP;
uniform vec3 cameraPosition;
uniform float size;

out vec2 texCoord;

void main() {
    vec3 pos = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(cameraPosition - pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up) * size; // 左手坐标系

    pos -= right;
    gl_Position = VP * vec4(pos, 1.0);
    texCoord = vec2(0.0, 0.0);
    EmitVertex();

    pos.y += size;
    gl_Position = VP * vec4(pos, 1.0);
    texCoord = vec2(0.0, 1.0);
    EmitVertex();

    pos.y -= size;
    pos += right;
    gl_Position = VP * vec4(pos, 1.0);
    texCoord = vec2(1.0, 0.0);
    EmitVertex();

    pos.y += size;
    gl_Position = VP * vec4(pos, 1.0);
    texCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
