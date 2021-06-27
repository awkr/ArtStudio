#pragma once

#include <glad/glad.h>

class Shader {
  public:
    ~Shader();
    bool createProgram(const char *vertFilename, const char *fragFilename);
    void deleteProgram();
    void use();
    void unuse();

    // an indexer that returns the location of the attibute / uniform
    GLuint operator[](const char *attr);
    GLuint operator()(const char *uniform);

  private:
    GLuint _program;
};
