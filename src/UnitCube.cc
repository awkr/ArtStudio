#include "UnitCube.h"
#include <glm/glm.hpp>

UnitCube::UnitCube() {
    _shader.createProgram("shaders/UnitCube.vert", "shaders/UnitCube.frag");

    init();
}

UnitCube::~UnitCube() {}

int UnitCube::getNumVertices() { return 8; }

int UnitCube::getNumIndices() {
    return 6 * 2 * 3; // 6 faces with 2 triangles each with 3 vertices
}

GLenum UnitCube::getPrimitiveType() { return GL_TRIANGLES; }

void UnitCube::fillVertexBuffer(GLfloat *buf) {
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

void UnitCube::fillIndexBuffer(GLuint *buf) {
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
