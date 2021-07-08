#include "Camera.h"
#include "Util.h"
#include <glm/gtc/quaternion.hpp>

Camera::Camera(const glm::vec3 &position, const glm::vec3 &target)
    : _position(position) {
    glm::vec3 forward = glm::normalize(target - position);
    glm::vec3 up(0, 1, 0);
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    up = glm::cross(right, forward); // re-compute up vector

    auto q = glm::quatLookAt(forward, up);
    auto angle = glm::degrees(glm::eulerAngles(q));
    _pitch = angle.x;
    _yaw = angle.y;
    _roll = angle.z;

    updateView();
    updateProjection();
}

Camera::~Camera() {}

void Camera::move(const glm::vec3 &offset) {
    _position += glm::vec3(_rotation * glm::vec4(offset, 0.0f));

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

void Camera::rotate(const glm::vec3 &angles) { debug("rotating"); }

void Camera::updateProjection() {
    _P = glm::perspective(glm::radians(_fovy), _aspect, _near, _far);
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
