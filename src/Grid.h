#pragma once

#include "RenderableObject.h"

class Grid : public RenderableObject {
  public:
    Grid(int width = 10, int depth = 10);
    virtual ~Grid();

    int getNumVertices() override;
    int getNumIndices() override;
    GLenum getPrimitiveType() override;

    void fillVertexBuffer(GLfloat *buf) override;
    void fillIndexBuffer(GLuint *buf) override;

  private:
    int _width, _depth;
};
