#pragma once

#include "RenderableObject.h"
#include <glm/glm.hpp>

class Quad : public RenderableObject {
  public:
    Quad(float zpos = 0);
    ~Quad();

    int getNumVertices() override;
    int getNumIndices() override;
    GLenum getPrimitiveType() override;
    inline glm::vec3 getPosition() const { return _position; }
    inline glm::vec3 getNormal() const { return _normal; }

    void fillVertexBuffer(GLfloat *buf) override;
    void fillIndexBuffer(GLuint *buf) override;

  private:
    float _zpos;
    glm::vec3 _position;
    glm::vec3 _normal;
};
