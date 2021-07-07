#pragma once

#include <RandomTexture.h>
#include <Shader.h>
#include <Texture.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

enum ParticleType { LAUNCHER, SHELL, SECONDARY_SHELL };

struct Particle {
    ParticleType type;
    glm::vec3 position;
    glm::vec3 velocity;
    int lifetime;
};

class ParticleSystem {
  public:
    ParticleSystem();
    ~ParticleSystem();

    bool init(const glm::vec3 &position);
    void render(double deltaTime, const glm::mat4 &VP,
                const glm::vec3 &cameraPosition);

  private:
    GLuint _transformFeedback[2];
    GLuint _buffer[2];
    double _time; // second
    unsigned int _bufferIndex;
    unsigned int _transformFeedbackIndex;
    Shader _shaderParticle;
    Shader _shaderBillboard;
    RandomTexture *_randomTex;
    Texture *_tex;
    bool _emitted;
};
