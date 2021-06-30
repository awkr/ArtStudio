#pragma once

#include "RenderableObject.h"

class UnitCube : public RenderableObject {
  public:
    UnitCube();
    virtual ~UnitCube();

    int getNumVertices() override;
    int getNumIndices() override;
    GLenum getPrimitiveType() override;

    void fillVertexBuffer(GLfloat *buf) override;
    void fillIndexBuffer(GLuint *buf) override;
};
