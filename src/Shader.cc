#include "Shader.h"
#include "Log.h"
#include <fstream>
#include <sstream>

static const GLchar *readShader(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    assert(fp);

    fseek(fp, 0, SEEK_END); // seek to end
    auto len = ftell(fp);

    fseek(fp, 0, SEEK_SET); // seek to start
    GLchar *source = new GLchar[len + 1];
    fread(source, 1, len, fp);

    fclose(fp);

    source[len] = 0;
    return const_cast<const GLchar *>(source);
}

static bool createShader(GLenum type, const char *filename, GLuint *shader) {
    *shader = glCreateShader(type);

    auto source = readShader(filename);
    glShaderSource(*shader, 1, &source, nullptr);
    delete[] source;

    glCompileShader(*shader);

    GLint status;
    if (glGetShaderiv(*shader, GL_COMPILE_STATUS, &status); status != GL_TRUE) {
        GLsizei len;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &len);

        GLchar *log = new GLchar[len + 1];
        glGetShaderInfoLog(*shader, len, &len, log);
        error("shader compiled error: %s", log);
        delete[] log;

        return false;
    }

    return true;
}

bool createProgram(const char *vertFilename, const char *fragFilename,
                   GLuint *prog) {
    GLuint vert, frag;
    assert(createShader(GL_VERTEX_SHADER, vertFilename, &vert));
    assert(createShader(GL_FRAGMENT_SHADER, fragFilename, &frag));

    *prog = glCreateProgram();

    glAttachShader(*prog, vert);
    glAttachShader(*prog, frag);

    glLinkProgram(*prog);
    GLint status;
    if (glGetProgramiv(*prog, GL_LINK_STATUS, &status); status != GL_TRUE) {
        GLsizei len;
        glGetProgramiv(*prog, GL_INFO_LOG_LENGTH, &len);

        GLchar *log = new GLchar[len + 1];
        glGetProgramInfoLog(*prog, len, &len, log);
        error("program linked error: %s", log);
        delete[] log;

        return false;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return true;
}
