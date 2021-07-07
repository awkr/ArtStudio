#include "Util.h"
#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include <random>
#include <sstream>

namespace as {

std::string to_string(const glm::vec3 &v) { return glm::to_string(v); }

std::string to_string(const glm::mat4 &m) { return glm::to_string(m); }

float randf() { return randf(0, 1); }

float randf(float min, float max) {
    static std::random_device rd;
    static std::default_random_engine eng(rd());
    return std::uniform_real_distribution<float>(min, max)(eng);
}

const GLchar *readfile(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        fatal("read file error: %s", filename);
    }

    fseek(f, 0, SEEK_END); // seek to end
    auto len = ftell(f);

    fseek(f, 0, SEEK_SET); // seek to start
    GLchar *source = new GLchar[len + 1];
    fread(source, 1, len, f);

    fclose(f);

    source[len] = 0;
    return const_cast<const GLchar *>(source);
}

void logGLInformation() {
    debug("Vendor: %s", glGetString(GL_VENDOR));
    debug("Renderer: %s", glGetString(GL_RENDERER));
    debug("Version: %s", glGetString(GL_VERSION));
    debug("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

} // namespace as
