#include "Camera.h"
#include "Util.h"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &target)
    : _position(position) {
    auto forward = glm::normalize(target - position);
    auto up = glm::vec3(0, 1, 0);
    auto right = glm::normalize(glm::cross(forward, up));
    up = glm::cross(right, forward);

    _rotation = glm::quatLookAt(forward, up);

    updateProjection();
}

Camera::~Camera() {}

glm::mat4 Camera::getV() {
    updateView();
    return _V;
}

void Camera::move(const glm::vec3 &offset) {
    _position += glm::vec3(_rotation * glm::vec4(offset, 0));
}

void Camera::pitch(const float angle) {
    if (_viewType == CAMERA) {
        // calc new orientation
        auto right = _rotation * glm::vec3(1, 0, 0);
        glm::quat q = glm::angleAxis(glm::radians(angle), right);
        _rotation = q * _rotation;
    } else {
        // calc new position
        glm::quat q = glm::angleAxis(glm::radians(angle), glm::vec3(1, 0, 0));
        _position = q * _position;

        // calc new orientation
        _rotation = q * _rotation;
    }
}

void Camera::yaw(const float angle) {
    if (_viewType == CAMERA) {
        // calc new orientation
        auto up = _rotation * glm::vec3(0, 1, 0);
        glm::quat q = glm::angleAxis(glm::radians(angle), up);
        _rotation = q * _rotation;
    } else {
        // calc new position
        glm::quat q = glm::angleAxis(glm::radians(angle), glm::vec3(0, 1, 0));
        _position = q * _position;

        // calc new orientation
        _rotation = q * _rotation;
    }
}

void Camera::roll(const float angle) {
    if (_viewType == CAMERA) {
        // calc new orientation
        auto forward = _rotation * glm::vec3(0, 0, 1);
        glm::quat q = glm::angleAxis(glm::radians(angle), forward);
        _rotation = q * _rotation;
    } else {
        // calc new position
        glm::quat q = glm::angleAxis(glm::radians(angle), glm::vec3(0, 0, 1));
        _position = q * _position;

        // calc new orientation
        _rotation = q * _rotation;
    }
}

void Camera::rotate(const double xoffset, const double yoffset) {
    if (_viewType == CAMERA) {
        // todo
    } else {
        // calc new position
        auto up = glm::vec3(0, 1, 0);
        auto right = _rotation * glm::vec3(1, 0, 0);

        glm::quat qy = glm::angleAxis(glm::radians(float(xoffset)), up);
        glm::quat qx = glm::angleAxis(glm::radians(float(yoffset)), right);

        auto q = glm::normalize(qy * qx);

        _position = q * _position;

        // calc new orientation
        _rotation = q * _rotation;
    }
}

void Camera::pan(const double xoffset, const double yoffset) {
    _position += _rotation * (glm::vec3(xoffset, yoffset, 0) * 0.1f);
}

void Camera::zoom(const double yoffset) {
    _position += _rotation * (glm::vec3(0, 0, yoffset) * 0.1f);
}

void Camera::setViewType(const CameraViewType t) { _viewType = t; }

void Camera::setAspect(const float aspect) {
    _aspect = aspect;
    updateProjection();
}

void Camera::rotate(const glm::vec3 &angles) {}

void Camera::updateView() {
    auto translation = glm::translate(glm::mat4(1), _position);
    auto rotation = glm::mat4_cast(_rotation);
    _V = glm::inverse(rotation) * glm::inverse(translation);
}

void Camera::updateProjection() {
    _P = glm::perspective(glm::radians(_fovy), _aspect, _near, _far);
}
