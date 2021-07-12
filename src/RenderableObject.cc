#include "RenderableObject.h"
#include <glm/gtc/type_ptr.hpp>

RenderableObject::RenderableObject(const glm::vec4 &color) : _color(color) {}

RenderableObject::~RenderableObject() {
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
    glBufferData(GL_ARRAY_BUFFER, getVerticesCount() * sizeof(glm::vec3), 0,
                 GL_STATIC_DRAW);
    auto fbuf =
        static_cast<GLfloat *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    initVertices(fbuf);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    auto loc = _shader.getAttribLocation("vertex");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eab);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndicesCount() * sizeof(GLuint), 0,
                 GL_STATIC_DRAW);
    auto ibuf = static_cast<GLuint *>(
        glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
    initIndices(ibuf);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    glBindVertexArray(0);
}

void RenderableObject::render(const float *MVP) {
    _shader.use();
    if (MVP) {
        _shader.setUniformMat4fv("MVP", MVP);
    }
    _shader.setUniform4fv("color", glm::value_ptr(_color));
    glBindVertexArray(_vao);
    glDrawElements(getPrimitiveType(), getIndicesCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    _shader.unuse();
}
