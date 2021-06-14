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
    std::strftime(buf, 19, "%Y/%m/%d %H:%M:%S", &localTime);

    // ms
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch());
    snprintf(buf, sizeof(buf), "%s.%03lld", buf, ms.count() % 1000);

    return buf;
}

#define log(fmt, args...)                                                      \
    {                                                                          \
        char buf[1024] = {0};                                                  \
        auto t = date();                                                       \
        snprintf(buf, sizeof(buf), "%s %s:%d#%s " fmt "\n", t.c_str(),         \
                 __FILE__, __LINE__, __FUNCTION__, ##args);                    \
        std::cout << buf;                                                      \
    }
