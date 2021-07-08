#include "Grid.h"
#include <glm/glm.hpp>

Grid::Grid(int width, int depth) : _width(width), _depth(depth) {
    _shader.init();
    _shader.attachVertexShader("Shaders/Common.vert");
    _shader.attachFragmentShader("Shaders/Common.frag");
    assert(_shader.link());

    init();
}

Grid::~Grid() {}

int Grid::getNumVertices() { return ((_width + 1) + (_depth + 1)) * 2; }

int Grid::getNumIndices() { return _width * _depth; }

GLenum Grid::getPrimitiveType() { return GL_LINES; }

void Grid::fillVertexBuffer(GLfloat *buf) {
    auto vertices = (glm::vec3 *)buf;

    int hw = _width / 2;
    int hd = _depth / 2;
    int index = 0;

    for (int i = -hw; i <= hw; ++i) {
        vertices[index++] = glm::vec3(i, 0, -hd);
        vertices[index++] = glm::vec3(i, 0, hd);

        vertices[index++] = glm::vec3(-hw, 0, i);
        vertices[index++] = glm::vec3(hw, 0, i);
    }
}

void Grid::fillIndexBuffer(GLuint *buf) {
    GLuint *id = buf;
    for (int i = 0; i < _width * _depth; i += 4) {
        *id++ = i;
        *id++ = i + 1;
        *id++ = i + 2;
        *id++ = i + 3;
    }
}
