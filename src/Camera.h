#pragma once

#include <glm/glm.hpp>

class Camera {
  public:
    Camera();
    Camera(float distance, float fovy, float aspect, float near, float far,
           float pitch, float yaw);
    ~Camera();

    glm::vec3 getPosition();
    inline float getDistance() const { return _distance; }
    inline float getPitch() const { return _pitch; }
    inline float getYaw() const { return _yaw; }
    inline glm::mat4 getP() const { return _P; }
    glm::mat4 getV() const;

    void rotate(const double xoffset, const double yoffset);
    void zoom(const double offset);

  private:
    // 位置
    float _distance = -10;

    // 视锥体
    float _fovy = 60.0f; // vertical FOV in degree
    float _aspect = 8.0f / 5.0f;
    float _near = 1.0f;
    float _far = 100.0f;

    // 旋转
    float _pitch = 0; // angle X in degree
    float _yaw = 45;  // angle Y in degree

    glm::mat4 _P; // projection matrix
};
