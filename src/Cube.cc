#include "Cube.h"
#include <glm/glm.hpp>

Cube::Cube() {
    _shader.init();
    _shader.attachVertexShader("Shaders/Cube.vert");
    _shader.attachFragmentShader("Shaders/Cube.frag");
    assert(_shader.link());

    auto color = glm::vec4(1, 0, 0, 1);

    _shader.use();
    _shader.setUniform4fv("color", &(color.x));
    _shader.unuse();

    init();
}

Cube::~Cube() {}

int Cube::getNumVertices() { return 8; }

int Cube::getNumIndices() {
    return 6 * 2 * 3; // 6 faces with 2 triangles each with 3 vertices
}

GLenum Cube::getPrimitiveType() { return GL_TRIANGLES; }

void Cube::fillVertexBuffer(GLfloat *buf) {
    auto vertices = (glm::vec3 *)buf;
    vertices[0] = glm::vec3(-0.5, -0.5, -0.5);
    vertices[1] = glm::vec3(0.5, -0.5, -0.5);
    vertices[2] = glm::vec3(0.5, 0.5, -0.5);
    vertices[3] = glm::vec3(-0.5, 0.5, -0.5);
    vertices[4] = glm::vec3(-0.5, -0.5, 0.5);
    vertices[5] = glm::vec3(0.5, -0.5, 0.5);
    vertices[6] = glm::vec3(0.5, 0.5, 0.5);
    vertices[7] = glm::vec3(-0.5, 0.5, 0.5);
}

void Cube::fillIndexBuffer(GLuint *buf) {
    GLuint *id = buf;

    // bottom face
    *id++ = 0;
    *id++ = 5;
    *id++ = 4;

    *id++ = 5;
    *id++ = 0;
    *id++ = 1;

    // top face
    *id++ = 3;
    *id++ = 7;
    *id++ = 6;

    *id++ = 3;
    *id++ = 6;
    *id++ = 2;

    // front face
    *id++ = 7;
    *id++ = 4;
    *id++ = 6;

    *id++ = 6;
    *id++ = 4;
    *id++ = 5;

    // back face
    *id++ = 2;
    *id++ = 1;
    *id++ = 3;

    *id++ = 3;
    *id++ = 1;
    *id++ = 0;

    // left face
    *id++ = 3;
    *id++ = 0;
    *id++ = 7;

    *id++ = 7;
    *id++ = 0;
    *id++ = 4;

    // right face
    *id++ = 6;
    *id++ = 5;
    *id++ = 2;

    *id++ = 2;
    *id++ = 5;
    *id++ = 1;
}
