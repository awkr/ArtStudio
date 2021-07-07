#include "Shader.h"
#include "Log.h"
#include "Util.h"

bool createShader(GLenum type, const char *filename, GLuint *shader) {
    *shader = glCreateShader(type);

    auto source = as::readfile(filename);
    glShaderSource(*shader, 1, &source, nullptr);
    delete[] source;

    glCompileShader(*shader);

    GLint status;
    if (glGetShaderiv(*shader, GL_COMPILE_STATUS, &status); status != GL_TRUE) {
        GLsizei len;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &len);

        GLchar *log = new GLchar[len];
        glGetShaderInfoLog(*shader, len, nullptr, log);
        error("shader compiled error: %s", log);
        delete[] log;

        return false;
    }

    return true;
}

Shader::Shader() {}

Shader::~Shader() { destory(); }

void Shader::destory() {
    for (unsigned i = 0; i < _vertexShaders.size(); ++i) {
        glDeleteShader(_vertexShaders[i]);
    }
    for (unsigned i = 0; i < _fragmentShaders.size(); ++i) {
        glDeleteShader(_fragmentShaders[i]);
    }
    for (unsigned i = 0; i < _geometryShaders.size(); ++i) {
        glDeleteShader(_geometryShaders[i]);
    }
    if (_program != 0) {
        glDeleteProgram(_program);
    }
}

void Shader::init() { _program = glCreateProgram(); }

bool Shader::createProgram(const char *vertFilename, const char *fragFilename,
                           const char *geomFilename) {
    GLuint vert, frag;
    assert(createShader(GL_VERTEX_SHADER, vertFilename, &vert));
    assert(createShader(GL_FRAGMENT_SHADER, fragFilename, &frag));

    glAttachShader(_program, vert);
    glAttachShader(_program, frag);

    if (geomFilename) {
        GLuint geom;
        assert(createShader(GL_GEOMETRY_SHADER, geomFilename, &geom));
        glAttachShader(_program, geom);
    }

    glLinkProgram(_program);
    GLint status;
    if (glGetProgramiv(_program, GL_LINK_STATUS, &status); status != GL_TRUE) {
        GLsizei len;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &len);

        GLchar *log = new GLchar[len];
        glGetProgramInfoLog(_program, len, nullptr, log);
        error("program linked error: %s", log);
        delete[] log;

        return false;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return true;
}

void Shader::attachVertexShader(const char *filename) {
    GLuint id;
    assert(createShader(GL_VERTEX_SHADER, filename, &id));
    _vertexShaders.push_back(id);
}

void Shader::attachFragmentShader(const char *filename) {
    GLuint id;
    assert(createShader(GL_FRAGMENT_SHADER, filename, &id));
    _fragmentShaders.push_back(id);
}

void Shader::attachGeometryShader(const char *filename) {
    GLuint id;
    assert(createShader(GL_GEOMETRY_SHADER, filename, &id));
    _geometryShaders.push_back(id);
}

bool Shader::link() {
    for (unsigned i = 0; i < _vertexShaders.size(); ++i) {
        glAttachShader(_program, _vertexShaders[i]);
    }
    for (unsigned i = 0; i < _geometryShaders.size(); ++i) {
        glAttachShader(_program, _geometryShaders[i]);
    }
    for (unsigned i = 0; i < _fragmentShaders.size(); ++i) {
        glAttachShader(_program, _fragmentShaders[i]);
    }

    glLinkProgram(_program);

    GLint status;
    if (glGetProgramiv(_program, GL_LINK_STATUS, &status); status != GL_TRUE) {
        GLsizei len;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &len);

        GLchar *log = new GLchar[len];
        glGetProgramInfoLog(_program, len, nullptr, log);
        error("program link error: %s", log);
        delete[] log;

        return false;
    }

    return true;
}

void Shader::deleteProgram() { glDeleteProgram(_program); }

void Shader::use() { glUseProgram(_program); }

void Shader::unuse() { glUseProgram(0); }

GLuint Shader::getAttribLocation(const char *name) {
    return glGetAttribLocation(_program, name);
}

GLuint Shader::getUniformLocation(const char *name) {
    return glGetUniformLocation(_program, name);
}

void Shader::setUniform1i(const char *name, int v) {
    glUniform1i(getUniformLocation(name), v);
}

void Shader::setUniform1f(const char *name, GLfloat v) {
    glUniform1f(getUniformLocation(name), v);
}

void Shader::setUniform3f(const char *name, const glm::vec3 &v) {
    glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
}

void Shader::setUniform3f(const char *name, GLfloat a, GLfloat b, GLfloat c) {
    glUniform3f(getUniformLocation(name), a, b, c);
}

void Shader::setUniform4fv(const char *name, const GLfloat *v, GLsizei n) {
    glUniform4fv(getUniformLocation(name), n, v);
}

void Shader::setUniformMat4fv(const char *name, const GLfloat *v, GLsizei n,
                              GLboolean transpose) {
    glUniformMatrix4fv(getUniformLocation(name), n, transpose, v);
}

void Shader::setTexture1D(const char *name, GLuint unit, int id) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_1D, id);
    setUniform1i(name, unit);
    glActiveTexture(GL_TEXTURE0);
}

void Shader::setTexture2D(const char *name, GLuint unit, int id) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
    setUniform1i(name, unit);
    glActiveTexture(GL_TEXTURE0);
}

void Shader::setTextureRect(const char *name, GLuint unit, int id) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_RECTANGLE, id);
    setUniform1i(name, unit);
    glActiveTexture(GL_TEXTURE0);
}
