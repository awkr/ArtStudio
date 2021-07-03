#include "Shader.h"
#include "Log.h"
#include <fstream>
#include <sstream>

static const GLchar *readFile(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        fatal("read file error: %s", filename);
    }

    fseek(f, 0, SEEK_END); // seek to end
    auto len = ftell(f);

    fseek(f, 0, SEEK_SET); // seek to start
    GLchar *source = new GLchar[len + 1];
    fread(source, 1, len, f);

    fclose(f);

    source[len] = 0;
    return const_cast<const GLchar *>(source);
}

static bool createShader(GLenum type, const char *filename, GLuint *shader) {
    *shader = glCreateShader(type);

    auto source = readFile(filename);
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

Shader::Shader() : _program(0) {}

Shader::~Shader() { destory(); }

void Shader::destory() {
    for (unsigned i = 0; i < _vertexShaders.size(); ++i) {
        glDeleteShader(_vertexShaders[i]);
    }
    for (unsigned i = 0; i < _fragmentShaders.size(); ++i) {
        glDeleteShader(_fragmentShaders[i]);
    }
    if (_program != 0) {
        glDeleteProgram(_program);
    }
}

bool Shader::createProgram(const char *vertFilename, const char *fragFilename,
                           const char *geomFilename) {
    GLuint vert, frag;
    assert(createShader(GL_VERTEX_SHADER, vertFilename, &vert));
    assert(createShader(GL_FRAGMENT_SHADER, fragFilename, &frag));

    _program = glCreateProgram();

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

bool Shader::link() {
    _program = glCreateProgram();

    for (unsigned i = 0; i < _vertexShaders.size(); ++i) {
        glAttachShader(_program, _vertexShaders[i]);
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

void Shader::setUniform4fv(const char *name, GLsizei n, const GLfloat *v) {
    glUniform4fv(getUniformLocation(name), n, v);
}

void Shader::setUniformMat4fv(const char *name, GLsizei n, GLboolean transpose,
                              const GLfloat *v) {
    glUniformMatrix4fv(getUniformLocation(name), n, transpose, v);
}

void Shader::setTextureRect(const char *name, GLuint texid, int texunit) {
    glActiveTexture(GL_TEXTURE0 + texunit);
    glBindTexture(GL_TEXTURE_RECTANGLE, texid);
    setUniform1i(name, texunit);
    glActiveTexture(GL_TEXTURE0);
}

GLuint Shader::operator[](const char *attr) {
    return glGetAttribLocation(_program, attr);
}

GLuint Shader::operator()(const char *uniform) {
    return glGetUniformLocation(_program, uniform);
}
