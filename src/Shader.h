#pragma once

#include <glad/glad.h>
#include <vector>

class Shader {
  public:
    Shader();
    ~Shader();

    void destory();

    bool createProgram(const char *vertFilename, const char *fragFilename,
                       const char *geomFilename = nullptr);
    void deleteProgram();

    void attachVertexShader(const char *filename);
    void attachFragmentShader(const char *filename);
    bool link();

    void use();
    void unuse();

    GLuint getAttribLocation(const char *name);
    GLuint getUniformLocation(const char *name);

    void setUniform1i(const char *name, int v);
    void setUniform1f(const char *name, GLfloat v);
    void setUniform4fv(const char *name, GLsizei n, const GLfloat *v);
    void setUniformMat4fv(const char *name, GLsizei n, GLboolean transpose,
                          const GLfloat *v);
    void setTextureRect(const char *name, GLuint texid, int texunit);

    // an indexer that returns the location of the attibute / uniform
    GLuint operator[](const char *attr);
    GLuint operator()(const char *uniform);

  private:
    std::vector<GLuint> _vertexShaders;
    std::vector<GLuint> _fragmentShaders;
    GLuint _program;
};
