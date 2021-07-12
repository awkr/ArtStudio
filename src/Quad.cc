#include "Quad.h"

Quad::Quad(float zpos) {
    _shader.createProgram("shaders/quad.vert", "shaders/quad.frag");

    _shader.use();
    glUniform1i(_shader.getUniformLocation("tex"), 0);
    _shader.unuse();

    _zpos = zpos;

    init();
}

Quad::~Quad() {}

int Quad::getVerticesCount() { return 4; }

int Quad::getIndicesCount() { return 6; /* 2 * 3 */ }

GLenum Quad::getPrimitiveType() { return GL_TRIANGLES; }

void Quad::initVertices(GLfloat *buf) {
    auto vertices = (glm::vec3 *)buf;
    vertices[0] = glm::vec3(-1, 0, _zpos);
    vertices[1] = glm::vec3(1, 0, _zpos);
    vertices[2] = glm::vec3(1, 2, _zpos);
    vertices[3] = glm::vec3(-1, 2, _zpos);

    auto center = vertices[0] + vertices[1] + vertices[2] + vertices[3];
    _position.x = center.x / 4.0f;
    _position.y = center.y / 4.0f;
    _position.z = center.z / 4.0f;

    auto v1 = vertices[1] - vertices[0];
    auto v2 = vertices[2] - vertices[0];
    _normal = glm::cross(v1, v2);
}

void Quad::initIndices(GLuint *buf) {
    GLuint *id = buf;

    *id++ = 0;
    *id++ = 1;
    *id++ = 2;

    *id++ = 0;
    *id++ = 2;
    *id++ = 3;
}
