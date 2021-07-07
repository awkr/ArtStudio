#include "ParticleSystem.h"
#include <Util.h>
#include <glm/gtc/type_ptr.hpp>

const int MAX_PARTICLES = 1000;

ParticleSystem::ParticleSystem() {}

ParticleSystem::~ParticleSystem() {}

bool ParticleSystem::init(const glm::vec3 &position) {
    Particle particles[MAX_PARTICLES];
    ZERO_MEM(particles);

    particles[0].type = LAUNCHER;
    particles[0].position = position;
    particles[0].velocity = glm::vec3(.0f, .1f, .0f);
    particles[0].lifetime = 0;

    glGenTransformFeedbacks(2, _transformFeedback);
    glGenBuffers(2, _buffer);

    for (unsigned int i = 0; i < 2; ++i) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _transformFeedback[i]);
        glBindBuffer(GL_ARRAY_BUFFER, _buffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles,
                     GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _buffer[i]);
    }

    _randomTex = new RandomTexture(1000);
    _tex = new Texture("fireworks_red.jpg");

    // init particle shader
    _shaderParticle.init();
    _shaderParticle.attachVertexShader("particle.vert");
    _shaderParticle.attachGeometryShader("particle.geom");
    _shaderParticle.attachFragmentShader("particle.frag");

    const GLchar *varyings[4];
    varyings[0] = "type1";
    varyings[1] = "position1";
    varyings[2] = "velocity1";
    varyings[3] = "age1";
    glTransformFeedbackVaryings(_shaderParticle.getPrograme(), 4, varyings,
                                GL_INTERLEAVED_ATTRIBS);
    assert(_shaderParticle.link());

    _shaderParticle.use();
    _shaderParticle.setTexture1D("randomTex", 3, _randomTex->getId());
    _shaderParticle.setUniform1f("launcherLifetime", 0.1);
    _shaderParticle.setUniform1f("shellLifetime", 10);
    _shaderParticle.setUniform1f("secondaryShellLifetime", 2.5);

    // init billboard shader
    _shaderBillboard.init();
    _shaderBillboard.attachVertexShader("billboard.vert");
    _shaderBillboard.attachGeometryShader("billboard.geom");
    _shaderBillboard.attachFragmentShader("billboard.frag");
    assert(_shaderBillboard.link());

    _shaderBillboard.use();
    _shaderBillboard.setTexture2D("tex", 0, _tex->getId());
    _shaderBillboard.setUniform1f("size", 0.02f);

    return true;
}

void ParticleSystem::render(double deltaTime, const glm::mat4 &VP,
                            const glm::vec3 &cameraPosition) {
    _time += deltaTime;

    // update
    _shaderParticle.use();
    _shaderParticle.setUniform1f("time", _time);
    _shaderParticle.setUniform1f("deltaTime", deltaTime);
    _shaderParticle.setTexture1D("randomTex", 3, _randomTex->getId());

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, _buffer[_bufferIndex]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,
                            _transformFeedback[_transformFeedbackIndex]);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
                          (const GLvoid *)offsetof(Particle, type));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
                          (const GLvoid *)offsetof(Particle, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
                          (const GLvoid *)offsetof(Particle, velocity));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
                          (const GLvoid *)offsetof(Particle, lifetime));
    glEnableVertexAttribArray(3);

    glBeginTransformFeedback(GL_POINTS);

    if (!_emitted) {
        glDrawArrays(GL_POINTS, 0, 1);
        _emitted = true;
    } else {
        glDrawTransformFeedback(GL_POINTS, _transformFeedback[_bufferIndex]);
    }

    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    // render
    _shaderBillboard.use();
    _shaderBillboard.setUniform3f("cameraPosition", cameraPosition);
    _shaderBillboard.setUniformMat4fv("VP", glm::value_ptr(VP));
    _shaderBillboard.setTexture2D("tex", 0, _tex->getId());

    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, _buffer[_transformFeedbackIndex]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
                          (const GLvoid *)offsetof(Particle, position));
    glDrawTransformFeedback(GL_POINTS,
                            _transformFeedback[_transformFeedbackIndex]);
    glDisableVertexAttribArray(0);

    _bufferIndex = _transformFeedbackIndex;
    _transformFeedbackIndex = (_transformFeedbackIndex + 1) & 0x1;
}
