#include "Camera.h"
#include "Util.h"

Camera::Camera(const vec3 &position, const vec3 &target) : _position(position) {
    _quat.set(1, 0, 0, 0);

    update_view_mats();

    setFrustum(60, 8.0 / 5.0, 1, 100);
}

Camera::~Camera() {}

void Camera::move(const vec3 &offsets) {
    _position += _quat.to_mat4() * offsets;

    update_view_mats();
}

void Camera::pitch(const float angle) {
    // the angle is for camera, so shoule be negated for computation
    auto q = quaternion::make(vec3(angle, 0, 0));
    _quat = q * _quat;

    update_view_mats();
}

void Camera::yaw(const float angle) {}

void Camera::roll(const float angle) {
    auto q = quaternion::make(vec3(0, 0, angle));
    _quat = q * _quat;

    update_view_mats();
}

void Camera::rotate(const vec3 &angles) {}

void Camera::setFrustum(float fovy, float aspect, float near, float far) {
    float tangent = tanf(deg2rad(fovy) * 0.5); // tangent of half fovy
    float height = near * tangent;             // half height of near plane
    float width = height * aspect;             // half width of near plane
    setFrustum(-width, width, -height, height, near, far);
}

void Camera::reset() {
    _quat.set(1, 0, 0, 0);
    _position = vec3(0, 5, 10);
    update_view_mats();
    setFrustum(60, 8.0 / 5.0, 1, 100);
}

// column-major
void Camera::setFrustum(float left, float right, float bottom, float top,
                        float near, float far) {
    auto proj = mat4();
    proj[0] = 2 * near / (right - left);
    proj[5] = 2 * near / (top - bottom);
    proj[2] = (right + left) / (right - left);
    proj[6] = (top + bottom) / (top - bottom);
    proj[10] = -(far + near) / (far - near);
    proj[14] = -1;
    proj[11] = -(2 * far * near) / (far - near);
    proj[15] = 0;
    _P = proj;
}

void Camera::update_view_mats() {
    auto R = _quat.inverse().to_mat4();
    auto T = mat4().translate(-_position);
    _V = R * T;
}
