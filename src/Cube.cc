#include "Cube.h"

Cube::Cube(const glm::vec4 &color) : RenderableObject(color) {
    _shader.init();
    _shader.attachVertexShader("Shaders/Cube.vert");
    _shader.attachFragmentShader("Shaders/Cube.frag");
    assert(_shader.link());

    init();
}

Cube::~Cube() {}

int Cube::getVerticesCount() { return 8; }

int Cube::getIndicesCount() {
    return 6 * 2 * 3; // 6 faces with 2 triangles each with 3 vertices
}

GLenum Cube::getPrimitiveType() { return GL_TRIANGLES; }

void Cube::initVertices(GLfloat *buf) {
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

void Cube::initIndices(GLuint *buf) {
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
