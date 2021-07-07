#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>

Camera::Camera() {
    _P = glm::perspective(glm::radians(_fovy), _aspect, _near, _far);
}

Camera::Camera(float distance, float fovy, float aspect, float near, float far,
               float pitch, float yaw)
    : _distance(distance), _fovy(fovy), _aspect(aspect), _near(near), _far(far),
      _pitch(pitch), _yaw(yaw) {
    _P = glm::perspective(glm::radians(_fovy), _aspect, _near, _far);
}

Camera::~Camera() {}

glm::vec3 Camera::getPosition() {
    auto v = glm::vec3(0, 0, _distance);
    return v;
}

glm::mat4 Camera::getV() const {
    auto T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, _distance));
    auto RxT =
        glm::rotate(T, glm::radians(_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    return glm::rotate(RxT, glm::radians(_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::rotate(const double xoffset, const double yoffset) {
    _yaw += xoffset;
    _pitch += yoffset;
}

void Camera::zoom(const double offset) { _distance -= offset; }
