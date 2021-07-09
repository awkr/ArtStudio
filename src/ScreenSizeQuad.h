#pragma once

#include "RenderableObject.h"

class ScreenSizeQuad : public RenderableObject {
  public:
    ScreenSizeQuad();
    virtual ~ScreenSizeQuad();

    int getVerticesCount() override;
    int getIndicesCount() override;
    GLenum getPrimitiveType() override;

    void initVertices(GLfloat *buf) override;
    void initIndices(GLuint *buf) override;
};
