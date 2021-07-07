#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Shader {
  public:
    Shader();
    ~Shader();

    void destory();
    void init();

    bool createProgram(const char *vertFilename, const char *fragFilename,
                       const char *geomFilename = nullptr);
    void deleteProgram();

    void attachVertexShader(const char *filename);
    void attachGeometryShader(const char *filename);
    void attachFragmentShader(const char *filename);
    bool link();

    void use();
    void unuse();

    inline GLuint getPrograme() const { return _program; }

    GLuint getAttribLocation(const char *name);
    GLuint getUniformLocation(const char *name);

    void setUniform1i(const char *name, int v);
    void setUniform1f(const char *name, GLfloat v);
    void setUniform3f(const char *name, const glm::vec3 &v);
    void setUniform3f(const char *name, GLfloat a, GLfloat b, GLfloat c);
    void setUniform4fv(const char *name, const GLfloat *v, GLsizei n = 1);
    void setUniformMat4fv(const char *name, const GLfloat *v, GLsizei n = 1,
                          GLboolean transpose = GL_FALSE);
    void setTexture1D(const char *name, GLuint unit, int id);
    void setTexture2D(const char *name, GLuint unit, int id);
    void setTextureRect(const char *name, GLuint unit, int id);

  private:
    std::vector<GLuint> _vertexShaders;
    std::vector<GLuint> _geometryShaders;
    std::vector<GLuint> _fragmentShaders;

    GLuint _program;
};
