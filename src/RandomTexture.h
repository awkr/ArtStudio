#pragma once

#include <glad/glad.h>

class RandomTexture {
  public:
    RandomTexture(unsigned int size);
    ~RandomTexture();

    void bind(GLenum unit);

    inline GLuint getId() const { return _id; }

  private:
    GLuint _id;
};
