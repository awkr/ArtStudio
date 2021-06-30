#include "RenderableObject.h"
#include <glm/glm.hpp>

RenderableObject::RenderableObject() {}

RenderableObject::~RenderableObject() { destory(); }

void RenderableObject::destory() {
    _shader.deleteProgram();

    glDeleteBuffers(1, &_eab);
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void RenderableObject::init() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_eab);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, getNumVertices() * sizeof(glm::vec3), 0,
                 GL_STATIC_DRAW);
    auto fbuf =
        static_cast<GLfloat *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    fillVertexBuffer(fbuf);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    auto loc = _shader.getAttribLocation("vertex");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eab);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getNumIndices() * sizeof(GLuint), 0,
                 GL_STATIC_DRAW);
    auto ibuf = static_cast<GLuint *>(
        glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
    fillIndexBuffer(ibuf);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    glBindVertexArray(0);
}

void RenderableObject::render(const float *MVP) {
    _shader.use();
    if (MVP) {
        glUniformMatrix4fv(_shader.getUniformLocation("MVP"), 1, GL_FALSE, MVP);
    }
    glBindVertexArray(_vao);
    glDrawElements(getPrimitiveType(), getNumIndices(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    _shader.unuse();
}
