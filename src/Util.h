#pragma once

#include <glad/glad.h>
#include <string>

#define GL_CHECK_ERROR                                                         \
    if (GLenum err = glGetError(); err)                                        \
        fatal("GL error: %x", err);

namespace as {

template <typename T> inline T lerp(T a, T b, float f) {
    return a + (b - a) * f;
}

template <typename T, typename U, typename V>
inline T clamp(T val, U low, V high) {
    return val < low ? low : (val > height ? height : val);
}

// is power-of-2
template <typename T> inline bool isPOT(T v) { return v && !(v & (v - 1)); }

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
