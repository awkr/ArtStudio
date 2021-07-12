#pragma once

#include "RenderableObject.h"

class Cube : public RenderableObject {
  public:
    Cube(const glm::vec4 &color = glm::vec4(1, 0, 0, 1));
    ~Cube();

    int getVerticesCount() override;
    int getIndicesCount() override;
    GLenum getPrimitiveType() override;

    void initVertices(GLfloat *buf) override;
    void initIndices(GLuint *buf) override;
};
