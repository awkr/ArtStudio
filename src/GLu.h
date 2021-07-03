#pragma once

// OpenGL utility

#define GL_CHECK_ERROR                                                         \
    {                                                                          \
        if (GLenum err = glGetError(); err)                                    \
            fatal("GL error: %x", err);                                        \
    }
