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

    void setUniform1i(const char *name, int v);
    void setUniform4fv(const char *name, GLsizei n, const GLfloat *v);
    void setUniformMat4fv(const char *name, GLsizei n, GLboolean transpose,
                          const GLfloat *v);

    // an indexer that returns the location of the attibute / uniform
    GLuint operator[](const char *attr);
    GLuint operator()(const char *uniform);

  private:
    GLuint _program;
};
