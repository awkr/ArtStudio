#include "ScreenSizeQuad.h"
#include <glm/glm.hpp>

ScreenSizeQuad::ScreenSizeQuad() {
    _shader.createProgram("shaders/ScreenSizeQuad.vert",
                          "shaders/ScreenSizeQuad.frag");
    _shader.use();
    glUniform1i(_shader.getUniformLocation("tex"), 0);
    _shader.unuse();

    // init VAO, VBO, EAB
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_eab);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, getNumVertices() * sizeof(glm::vec2), 0,
                 GL_STATIC_DRAW);
    auto fbuf =
        static_cast<GLfloat *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    fillVertexBuffer(fbuf);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    auto loc = _shader.getAttribLocation("vertex");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eab);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getNumIndices() * sizeof(GLuint), 0,
                 GL_STATIC_DRAW);
    auto ibuf = static_cast<GLuint *>(
        glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
    fillIndexBuffer(ibuf);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    glBindVertexArray(0);
}

ScreenSizeQuad::~ScreenSizeQuad() {}

int ScreenSizeQuad::getNumVertices() { return 4; }

int ScreenSizeQuad::getNumIndices() { return 6; }

GLenum ScreenSizeQuad::getPrimitiveType() { return GL_TRIANGLES; }

void ScreenSizeQuad::fillVertexBuffer(GLfloat *buf) {
    auto vertices = (glm::vec2 *)buf;
    vertices[0] = glm::vec2(-1, -1);
    vertices[1] = glm::vec2(1, -1);
    vertices[2] = glm::vec2(1, 1);
    vertices[3] = glm::vec2(-1, 1);
}

void ScreenSizeQuad::fillIndexBuffer(GLuint *buf) {
    GLuint *id = buf;

    *id++ = 0;
    *id++ = 1;
    *id++ = 2;

    *id++ = 0;
    *id++ = 2;
    *id++ = 3;
}
