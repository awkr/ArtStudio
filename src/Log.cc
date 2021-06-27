#include "Log.h"
#include <chrono>
#include <iostream>
#include <string>

namespace at {

static const char *GREEN = "\033[0;32m";
static const char *YELLOW = "\033[0;33m";
static const char *RED = "\033[0;31m";
static const char *PURPLE = "\033[0;35m";
static const char *GREY = "\033[0;90m";
static const char *NC = "\033[0m"; // no color

static std::string datestr() {
    auto now = std::chrono::system_clock::now();

    // datetime
    auto t = std::chrono::system_clock::to_time_t(now);
    struct tm localTime;
    localtime_r(&t, &localTime);
    char buf[25];
    std::strftime(buf, 19, "%y/%m/%d %H:%M:%S", &localTime);

    // ms
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch());
    snprintf(buf, sizeof(buf), "%s.%03lld", buf, ms.count() % 1000);

    return buf;
}

void log(const char *file, unsigned int line, const char *func, char level,
         const char *format, ...) {
    auto color = NC;
    switch (level) {
    case 'I':
        color = GREEN;
        break;
    case 'W':
        color = YELLOW;
        break;
    case 'E':
        color = RED;
        break;
    case 'F':
        color = PURPLE;
        break;
    default:
        level = 'D';
    }

    fprintf(stdout, "%s %s%c %s%s:%d#%s%s ", datestr().c_str(), color, level,
            GREY, file, line, func, NC);

    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);

    fprintf(stdout, "\n");

    if (level == 'F')
        exit(EXIT_FAILURE);
}

} // namespace at
