#pragma once

#include <glad/glad.h>

class Shader {
  public:
    ~Shader();
    bool createProgram(const char *vertFilename, const char *fragFilename,
                       const char *geomFilename = nullptr);
    void deleteProgram();
    void use();
    void unuse();

    GLuint getAttribLocation(const char *name);
    GLuint getUniformLocation(const char *name);

    // an indexer that returns the location of the attibute / uniform
    GLuint operator[](const char *attr);
    GLuint operator()(const char *uniform);

  private:
    GLuint _program;
};
