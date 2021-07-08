#pragma once

#include <glm/glm.hpp>

class Camera {
  public:
    Camera(const glm::vec3 &position, const glm::vec3 &target = glm::vec3(.0f));
    ~Camera();

    inline glm::vec3 getPosition() const { return _position; }
    inline glm::mat4 getV() const { return _V; };
    inline glm::mat4 getP() const { return _P; }

    // move along the camera's axis
    void move(const glm::vec3 &offset);

    // rotate around camera's axis
    void pitch(const float angle);
    void yaw(const float angle);
    void roll(const float angle);

    void rotate(const glm::vec3 &angles);

  private:
    void updateView();
    void updateProjection();

    glm::vec3 _position;
    glm::mat4 _rotation;

    // axis
    glm::vec3 _right;
    glm::vec3 _up;
    glm::vec3 _forward;

    // angles in degree
    float _pitch;
    float _yaw;
    float _roll;

    // forstum
    float _fovy = 60.0;        // vertical FOV in degree
    float _aspect = 8.0 / 5.0; // classical 16:10
    float _near = 1.0;
    float _far = 100.0;

    glm::mat4 _V; // view matrix
    glm::mat4 _P; // projection matrix
};
