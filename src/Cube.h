#pragma once

#include "RenderableObject.h"

class Cube : public RenderableObject {
  public:
    Cube();
    ~Cube();

    int getNumVertices() override;
    int getNumIndices() override;
    GLenum getPrimitiveType() override;

    void fillVertexBuffer(GLfloat *buf) override;
    void fillIndexBuffer(GLuint *buf) override;
};
