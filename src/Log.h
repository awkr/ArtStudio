#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace at {

void log(const char *file, unsigned int line, const char *func, char level,
         const char *format, ...);

}

#define debug(fmt, args...)                                                    \
    at::log(__FILE__, __LINE__, __FUNCTION__, 'D', fmt, ##args);

#define info(fmt, args...)                                                     \
    at::log(__FILE__, __LINE__, __FUNCTION__, 'I', fmt, ##args);

#define warn(fmt, args...)                                                     \
    at::log(__FILE__, __LINE__, __FUNCTION__, 'W', fmt, ##args);

#define error(fmt, args...)                                                    \
    at::log(__FILE__, __LINE__, __FUNCTION__, 'E', fmt, ##args);

#define fatal(fmt, args...)                                                    \
    at::log(__FILE__, __LINE__, __FUNCTION__, 'F', fmt, ##args);
