#pragma once

#include <glad/glad.h>

bool createProgram(const char *vertFilename, const char *fragFilename,
                   GLuint *prog);
