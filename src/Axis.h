#pragma once

#include "RenderableObject.h"

class Axis : public RenderableObject {
  public:
    Axis(const int length = 5);
    ~Axis();

    int getVerticesCount() override;
    int getIndicesCount() override;
    GLenum getPrimitiveType() override;

    void initVertices(GLfloat *buf) override;
    void initIndices(GLuint *buf) override;

  private:
    int _length;
};
