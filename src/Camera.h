#pragma once

#include "mathematics.h"

class Camera {
  public:
    Camera(const vec3 &position, const vec3 &target = vec3());
    ~Camera();

    inline vec3 getPosition() const { return _position; }
    inline mat4 getV() const { return _V; };
    inline mat4 getP() const { return _P; }

    // move along the camera's axis
    void move(const vec3 &offsets);

    // rotate around camera's axis
    void pitch(const float angle);
    void yaw(const float angle);
    void roll(const float angle);

    void rotate(const vec3 &angles);

    void setFrustum(float fovy, float aspect, float near, float far);

    void reset();

  private:
    void setFrustum(float left, float right, float bottom, float top,
                    float near, float far);

    void update_view_mats();

    vec3 _position;
    quaternion _quat;

    // 视锥体
    float _fovy = 60.0;        // vertical FOV in degree
    float _aspect = 8.0 / 5.0; // classical 16:10
    float _near = 1.0;
    float _far = 100.0;

    mat4 _V; // view matrix
    mat4 _P; // projection matrix
};
