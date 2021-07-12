#include "Axis.h"

Axis::Axis(const int length) : _length(length) {
    _shader.init();
    _shader.attachVertexShader("Shaders/Axis.vert");
    _shader.attachFragmentShader("Shaders/Axis.frag");
    assert(_shader.link());

    init();
}

Axis::~Axis() {}

int Axis::getVerticesCount() { return 6; }

int Axis::getIndicesCount() { return 6; }

GLenum Axis::getPrimitiveType() { return GL_LINES; }

void Axis::initVertices(GLfloat *buf) {
    auto vertices = (glm::vec3 *)buf;

    // axis x
    vertices[0] = glm::vec3(_length, 0, 0);
    vertices[1] = glm::vec3(0, 0, 0);
    // axis y
    vertices[2] = glm::vec3(0, _length, 0);
    vertices[3] = glm::vec3(0, 0, 0);
    // axis z
    vertices[4] = glm::vec3(0, 0, _length);
    vertices[5] = glm::vec3(0, 0, 0);
}

void Axis::initIndices(GLuint *buf) {
    GLuint *id = buf;
    *id++ = 0;
    *id++ = 1;
    *id++ = 2;
    *id++ = 3;
    *id++ = 4;
    *id = 5;
}
