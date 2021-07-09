#pragma once

#include "RenderableObject.h"

class Grid : public RenderableObject {
  public:
    Grid(int width = 10, int depth = 10,
         const glm::vec4 &color = glm::vec4(0.7, 0.7, 0.7, 1));
    virtual ~Grid();

    int getVerticesCount() override;
    int getIndicesCount() override;
    GLenum getPrimitiveType() override;

    void initVertices(GLfloat *buf) override;
    void initIndices(GLuint *buf) override;

  private:
    int _width, _depth;
};
