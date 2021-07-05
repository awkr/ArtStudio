#pragma once

#include <glad/glad.h>
#include <string>

#define GL_CHECK_ERROR                                                         \
    if (GLenum err = glGetError(); err)                                        \
        fatal("GL error: %x", err);

namespace as {

GLfloat lerp(GLfloat a, GLfloat b, GLfloat f) { return a + f * (b - a); }

template <typename... Args>
std::string stringf(const char *format, Args... args) {
    int length = std::snprintf(nullptr, 0, format, args...);
    char *buf = new char[length + 1];
    std::snprintf(buf, length + 1, format, args...);
    std::string s(buf);
    delete[] buf;
    return s;
}

} // namespace as
