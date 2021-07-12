#pragma once

#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class RenderableObject {
  public:
    RenderableObject(const glm::vec4 &color = glm::vec4(1));
    virtual ~RenderableObject();

    virtual void initVertices(GLfloat *buf) = 0;
    virtual void initIndices(GLuint *buf) = 0;

    // initialize vertex array object, vertex buffer object, element array
    // buffer
    void init();

    void render(const float *MVP = nullptr);

    virtual int getVerticesCount() = 0;
    virtual int getIndicesCount() = 0;
    virtual GLenum getPrimitiveType() = 0;

  protected:
    GLuint _vao;
    GLuint _vbo;
    GLuint _eab;

    Shader _shader;
    glm::vec4 _color;
};
