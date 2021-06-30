#include "Plane.h"

const float EPSILON = 0.000001f;

Plane::Plane() {}

Plane::Plane(const glm::vec3 &normal, const glm::vec3 &p) {
    _normal = normal;
    _d = -glm::dot(normal, p);
}

Plane::~Plane() {}

Plane Plane::fromPoints(const glm::vec3 &a, const glm::vec3 &b,
                        const glm::vec3 &c) {
    auto v1 = b - a;
    auto v2 = c - a;
    auto normal = glm::normalize(glm::cross(v1, v2));
    return Plane(normal, a);
}

Where Plane::classify(const glm::vec3 &p) {
    if (float d = getDistance(p); d > EPSILON) {
        return FRONT;
    } else if (d < EPSILON) {
        return BACK;
    }
    return COPLANAR;
}

float Plane::getDistance(const glm::vec3 &p) {
    return glm::dot(_normal, p) + _d;
}
