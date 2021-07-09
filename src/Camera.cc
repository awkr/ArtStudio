#include "Camera.h"
#include "Util.h"
#include <glm/gtc/quaternion.hpp>

Camera::Camera(const glm::vec3 &position) : _position(position) {
    lookAt(glm::vec3(0), glm::vec3(0, 1, 0));
    updateProjection();
}

Camera::~Camera() {}

glm::mat4 Camera::getV() {
    auto translation = glm::translate(glm::mat4(1), _position);

    auto quat = glm::quat(glm::radians(glm::vec3(_pitch, _yaw, _roll)));
    auto rotation = glm::mat4_cast(quat);

    return glm::inverse(rotation) * glm::inverse(translation);
}

void Camera::move(const glm::vec3 &offset) {
    _position += glm::vec3(_rotation * glm::vec4(offset * _moveSpeed, 0.0f));
    updateView();
}

void Camera::pitch(const float angle) {
    _pitch += angle;
    updateView();
}

void Camera::yaw(const float angle) {
    _yaw += angle;
    updateView();
}

void Camera::roll(const float angle) {
    _roll += angle;
    updateView();
}

void Camera::rotate(const double xoffset, const double yoffset) {
    _yaw += xoffset;
    _pitch += yoffset;
}

void Camera::rotate(const glm::vec3 &angles) {
    _pitch += angles.x * _rotateSpeed;
    _yaw += angles.y * _rotateSpeed;
    updateView();
}

void Camera::rotateBy(const glm::vec3 &target, const glm::vec3 &offset) {
    _forward = glm::normalize(target - _position);
    _right = glm::normalize(glm::cross(_forward, glm::vec3(0, 1, 0)));
    _up = glm::cross(_right, _forward);

    auto q = glm::quatLookAt(_forward, _up);
    _rotation = glm::mat4_cast(q);

    debug("pos %s, offset %s", as::to_string(_position).c_str(),
          as::to_string(offset).c_str());

    _position += glm::vec3(_rotation * glm::vec4(offset * _moveSpeed, 0.0f));

    auto angle = glm::degrees(glm::eulerAngles(q));
    _pitch = angle.x;
    _yaw = angle.y;
    _roll = angle.z;

    // updateView();

    {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), _position);

        // update view matrix
        _V = glm::inverse(_rotation) * glm::inverse(translate);
    }
}

void Camera::lookAt(const glm::vec3 &target) { lookAt(target, _up); }

void Camera::setAspect(const float aspect) {
    _aspect = aspect;
    updateProjection();
}

void Camera::lookAt(const glm::vec3 &target, const glm::vec3 &up) {
    glm::vec3 forward = glm::normalize(target - _position);
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    glm::vec3 y = glm::cross(right, forward);

    auto q = glm::quatLookAt(forward, y);
    auto angle = glm::degrees(glm::eulerAngles(q));
    _pitch = angle.x;
    _yaw = angle.y;
    _roll = angle.z;

    updateView();
}

void Camera::updateView() {
    glm::quat pitch = glm::angleAxis(glm::radians(_pitch), glm::vec3(1, 0, 0));
    glm::quat yaw = glm::angleAxis(glm::radians(_yaw), glm::vec3(0, 1, 0));
    glm::quat roll = glm::angleAxis(glm::radians(_roll), glm::vec3(0, 0, 1));

    auto orientation = glm::normalize(roll * yaw * pitch);
    _rotation = glm::mat4_cast(orientation);

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), _position);

    // update axis
    _forward = glm::vec3(_rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    _up = glm::vec3(_rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    _right = glm::vec3(_rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    // update view matrix
    _V = glm::inverse(_rotation) * glm::inverse(translate);
}

void Camera::updateProjection() {
    _P = glm::perspective(glm::radians(_fovy), _aspect, _near, _far);
}
