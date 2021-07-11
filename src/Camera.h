#pragma once

// 交互设计：
// 1 光标控制：锚点为物体
// 2 键盘控制：锚点为相机

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

enum CameraViewType { CAMERA, THIRD_PERSON };

class Camera {
  public:
    Camera(const glm::vec3 &position, const glm::vec3 &target = glm::vec3(0));
    ~Camera();

    inline glm::vec3 getPosition() const { return _position; }
    glm::mat4 getV();
    inline glm::mat4 getP() const { return _P; }

    // move along the camera's axis
    void move(const glm::vec3 &offset);

    // rotate around camera's axis
    void pitch(const float angle);
    void yaw(const float angle);
    void roll(const float angle);

    void rotate(const glm::vec3 &angles);

    //
    void rotate(const double xoffset, const double yoffset);

    void pan(const double xoffset, const double yoffset);
    void zoom(const double yoffset);

    void reset();

    void setViewType(const CameraViewType t);
    void setAspect(const float aspect);

  private:
    void lookAt(const glm::vec3 &target);

    void updateView();
    void updateProjection();

    glm::vec3 _initPosition;
    glm::vec3 _position;
    glm::quat _rotation;
    glm::vec3 _target;

    // forstum
    float _fovy = 60.0;        // vertical FOV in degree
    float _aspect = 8.0 / 5.0; // classical 16:10
    float _near = 1.0;
    float _far = 100.0;

    glm::mat4 _V; // view matrix
    glm::mat4 _P; // projection matrix

    // controls how fast the rotation is.
    // mainly used for the rotation controlled by the cursor.
    float _rotateSpeed = 0.75f;
    float _moveSpeed = 0.04f;

    CameraViewType _viewType = CAMERA;
};
