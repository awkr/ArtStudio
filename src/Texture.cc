#include "Texture.h"
#include "Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char *filename) {
    int width, height, channel;
    unsigned char *data = stbi_load(filename, &width, &height, &channel, 0);
    if (!data)
        fatal("load image error: %s", filename);

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    debug("texture created: %d", _id);
}

Texture::~Texture() {
    if (_id > 0)
        glDeleteTextures(1, &_id);
}

void Texture::bind(GLenum unit) {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}
