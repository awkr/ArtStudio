#pragma once

#include "RenderableObject.h"

class ScreenSizeQuad : public RenderableObject {
  public:
    ScreenSizeQuad();
    virtual ~ScreenSizeQuad();

    int getNumVertices() override;
    int getNumIndices() override;
    GLenum getPrimitiveType() override;

    void fillVertexBuffer(GLfloat *buf) override;
    void fillIndexBuffer(GLuint *buf) override;
};
