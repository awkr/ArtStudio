#pragma once

#include <glm/glm.hpp>

enum Where { COPLANAR, FRONT, BACK };

// Ax + By + Cz + d = 0
class Plane {
  public:
    Plane();
    Plane(const glm::vec3 &normal, const glm::vec3 &p);
    ~Plane();

    static Plane fromPoints(const glm::vec3 &a, const glm::vec3 &b,
                            const glm::vec3 &c);
    Where classify(const glm::vec3 &p);
    // 点 p 到平面的距离
    float getDistance(const glm::vec3 &p);

  private:
    glm::vec3 _normal = glm::vec3(0, 1, 0); // A B C
    float _d = 0; // d，即将平面移到坐标原点所需距离
};
