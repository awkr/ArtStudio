#version 330 core

layout (points) in;
layout (points) out;
layout (max_vertices = 30) out;

in float type0[];
in vec3 position0[];
in vec3 velocity0[];
in float age0[];

out float type1;
out vec3 position1;
out vec3 velocity1;
out float age1;

uniform float deltaTime;
uniform float time;
uniform sampler1D randomTex;
uniform float launcherLifetime;
uniform float shellLifetime;
uniform float secondaryShellLifetime;

#define PARTICLE_TYPE_LAUNCHER 0
#define PARTICLE_TYPE_SHELL 1
#define PARTICLE_TYPE_SECONDARY_SHELL 2

vec3 getRandomDir(float texCoord) {
     vec3 dir = texture(randomTex, texCoord).xyz;
     dir -= vec3(0.5, 0.5, 0.5);
     return dir;
}

void main() {
    float age = age0[0] + deltaTime;

    if (type0[0] == PARTICLE_TYPE_LAUNCHER) {
        if (age >= launcherLifetime) {
            type1 = PARTICLE_TYPE_SHELL;
            position1 = position0[0];
            vec3 dir = getRandomDir(time);
            dir.y = max(dir.y, 0.5);
            velocity1 = normalize(dir) / 20.0;
            age1 = 0.0;
            EmitVertex();
            EndPrimitive();
            age = 0.0;
        }

        type1 = PARTICLE_TYPE_LAUNCHER;
        position1 = position0[0];
        velocity1 = velocity0[0];
        age1 = age;
        EmitVertex();
        EndPrimitive();
    } else {
        vec3 deltaP = deltaTime * velocity0[0];
        vec3 deltaV = vec3(deltaTime) * (0.0, -9.81, 0.0);

        if (type0[0] == PARTICLE_TYPE_SHELL)  {
	        if (age < shellLifetime) {
	            type1 = PARTICLE_TYPE_SHELL;
	            position1 = position0[0] + deltaP;
	            velocity1 = velocity0[0] + deltaV;
	            age1 = age;
	            EmitVertex();
	            EndPrimitive();
	        } else {
                for (int i = 0 ; i < 10 ; i++) {
                     type1 = PARTICLE_TYPE_SECONDARY_SHELL;
                     position1 = position0[0];
                     vec3 dir = getRandomDir(time + i);
                     velocity1 = normalize(dir) / 20.0;
                     age1 = 0.0f;
                     EmitVertex();
                     EndPrimitive();
                }
            }
        } else {
            if (age < secondaryShellLifetime) {
                type1 = PARTICLE_TYPE_SECONDARY_SHELL;
                position1 = position0[0] + deltaP;
                velocity1 = velocity0[0] + deltaV;
                age1 = age;
                EmitVertex();
                EndPrimitive();
            }
        }
    }
}
