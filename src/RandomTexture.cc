#include "RandomTexture.h"
#include "Util.h"
#include <glm/glm.hpp>

RandomTexture::RandomTexture(unsigned int size) : _id(0) {
    glm::vec3 *data = new glm::vec3[size];
    for (unsigned int i = 0; i < size; ++i) {
        data[i].x = as::randf();
        data[i].y = as::randf();
        data[i].z = as::randf();
    }

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_1D, _id);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, data);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    delete[] data;

    GL_CHECK_ERROR

    debug("random texture created: %d", _id);
}

RandomTexture::~RandomTexture() {
    if (_id > 0)
        glDeleteTextures(1, &_id);
}

void RandomTexture::bind(GLenum unit) {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_1D, _id);
}
