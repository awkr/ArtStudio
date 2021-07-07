#pragma once

#include <glad/glad.h>

class Texture {
  public:
    Texture(const char *filename);
    ~Texture();

    void bind(GLenum unit);

    inline GLuint getId() const { return _id; }

  private:
    GLuint _id;
};
