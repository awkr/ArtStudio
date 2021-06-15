#pragma once

#include <chrono>
#include <iostream>
#include <string>

static std::string date() {
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

const char *RED = "\033[0;31m";
const char *GREEN = "\033[0;32m";
const char *YELLOW = "\033[0;33m";
const char *NC = "\033[0m"; // no color

#define debug(fmt, args...)                                                    \
    fprintf(stdout, "%s debug %s:%d#%s " fmt "\n", date().c_str(), __FILE__,   \
            __LINE__, __FUNCTION__, ##args);

#define info(fmt, args...)                                                     \
    fprintf(stdout, "%s %sinfo%s %s:%d#%s " fmt "\n", date().c_str(), GREEN,   \
            NC, __FILE__, __LINE__, __FUNCTION__, ##args);

#define warn(fmt, args...)                                                     \
    fprintf(stdout, "%s %swarn%s %s:%d#%s " fmt "\n", date().c_str(), YELLOW,  \
            NC, __FILE__, __LINE__, __FUNCTION__, ##args);

#define error(fmt, args...)                                                    \
    fprintf(stdout, "%s %serror%s %s:%d#%s " fmt "\n", date().c_str(), RED,    \
            NC, __FILE__, __LINE__, __FUNCTION__, ##args);
