#pragma once

#include <string>

namespace as {

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
