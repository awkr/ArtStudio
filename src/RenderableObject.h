#pragma once

#include "Shader.h"
#include <glad/glad.h>

class RenderableObject {
  public:
    RenderableObject();
    virtual ~RenderableObject();

    void init();
    void render(const float *MVP = nullptr);

    virtual int getNumVertices() = 0;
    virtual int getNumIndices() = 0;
    virtual GLenum getPrimitiveType() = 0;

    virtual void fillVertexBuffer(GLfloat *buf) = 0;
    virtual void fillIndexBuffer(GLuint *buf) = 0;

  protected:
    GLuint _vao;
    GLuint _vbo;
    GLuint _eab;

    Shader _shader;

    void destory();
};
