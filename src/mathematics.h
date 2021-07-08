#pragma once

#include <cmath>
#include <iomanip>
#include <ostream>

const float PI = 3.141593;
const float DEG_TO_RAD = PI / 180.0;
const float EPSILON = 0.00001f;

inline float deg2rad(float deg) { return deg * DEG_TO_RAD; }

struct vec3 {
    float x, y, z;

    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    vec3 &normalize();
    float dot(const vec3 &rhs) const;
    vec3 cross(const vec3 &rhs) const;

    vec3 operator-() const;
    vec3 operator*(const float a) const;
    vec3 operator+(const vec3 &rhs) const;
    vec3 operator-(const vec3 &rhs) const;
    vec3 &operator+=(const vec3 &rhs);

    friend vec3 operator*(const float a, const vec3 &v);
    friend std::ostream &operator<<(std::ostream &os, const vec3 &v);
};

inline vec3 &vec3::normalize() {
    float d = x * x + y * y + z * z;
    if (d < EPSILON)
        return *this; // do nothing if it is zero
    d = 1.0f / sqrtf(d);
    x *= d;
    y *= d;
    z *= d;
    return *this;
}

inline float vec3::dot(const vec3 &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

inline vec3 vec3::cross(const vec3 &rhs) const {
    return vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z,
                x * rhs.y - y * rhs.x);
}

inline vec3 vec3::operator-() const { return vec3(-x, -y, -z); }

inline vec3 vec3::operator*(const float a) const {
    return vec3(x * a, y * a, z * a);
}

inline vec3 vec3::operator+(const vec3 &rhs) const {
    return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

inline vec3 vec3::operator-(const vec3 &rhs) const {
    return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline vec3 &vec3::operator+=(const vec3 &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

inline vec3 operator*(const float a, const vec3 &v) {
    return vec3(a * v.x, a * v.y, a * v.z);
}

inline std::ostream &operator<<(std::ostream &os, const vec3 &v) {
    os << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

struct mat4 { // column-major
    float m[16];

    mat4();                                          // init with identity
    mat4(float m00, float m01, float m02, float m03, // 1st column
         float m04, float m05, float m06, float m07, // 2nd column
         float m08, float m09, float m10, float m11, // 3rd column
         float m12, float m13, float m14, float m15  // 4th column
    );

    mat4 &transpose();

    // transform matrix
    mat4 &translate(const vec3 &v);
    mat4 &translate(float x, float y, float z);

    mat4 operator*(const mat4 &rhs) const;
    vec3 operator*(const vec3 &rhs) const;
    float operator[](int index) const;
    float &operator[](int index);

    friend std::ostream &operator<<(std::ostream &os, const mat4 &m);
};

inline mat4::mat4() {
    m[0] = m[5] = m[10] = m[15] = 1.0f;
    m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] =
        m[13] = m[14] = 0.0f;
}

inline mat4::mat4(float m00, float m01, float m02, float m03, // 1st column
                  float m04, float m05, float m06, float m07, // 2nd column
                  float m08, float m09, float m10, float m11, // 3rd column
                  float m12, float m13, float m14, float m15  // 4th column
) {
    m[0] = m00;
    m[1] = m01;
    m[2] = m02;
    m[3] = m03;
    m[4] = m04;
    m[5] = m05;
    m[6] = m06;
    m[7] = m07;
    m[8] = m08;
    m[9] = m09;
    m[10] = m10;
    m[11] = m11;
    m[12] = m12;
    m[13] = m13;
    m[14] = m14;
    m[15] = m15;
}

inline mat4 &mat4::transpose() {
    std::swap(m[1], m[4]);
    std::swap(m[2], m[8]);
    std::swap(m[3], m[12]);
    std::swap(m[6], m[9]);
    std::swap(m[7], m[13]);
    std::swap(m[11], m[14]);
    return *this;
}

inline mat4 &mat4::translate(const vec3 &v) { return translate(v.x, v.y, v.z); }

inline mat4 &mat4::translate(float x, float y, float z) {
    m[0] += m[3] * x;
    m[4] += m[7] * x;
    m[8] += m[11] * x;
    m[12] += m[15] * x;

    m[1] += m[3] * y;
    m[5] += m[7] * y;
    m[9] += m[11] * y;
    m[13] += m[15] * y;

    m[2] += m[3] * z;
    m[6] += m[7] * z;
    m[10] += m[11] * z;
    m[14] += m[15] * z;

    return *this;
}

inline mat4 mat4::operator*(const mat4 &rhs) const {
    return mat4(
        m[0] * rhs[0] + m[4] * rhs[1] + m[8] * rhs[2] + m[12] * rhs[3],
        m[1] * rhs[0] + m[5] * rhs[1] + m[9] * rhs[2] + m[13] * rhs[3],
        m[2] * rhs[0] + m[6] * rhs[1] + m[10] * rhs[2] + m[14] * rhs[3],
        m[3] * rhs[0] + m[7] * rhs[1] + m[11] * rhs[2] + m[15] * rhs[3],
        m[0] * rhs[4] + m[4] * rhs[5] + m[8] * rhs[6] + m[12] * rhs[7],
        m[1] * rhs[4] + m[5] * rhs[5] + m[9] * rhs[6] + m[13] * rhs[7],
        m[2] * rhs[4] + m[6] * rhs[5] + m[10] * rhs[6] + m[14] * rhs[7],
        m[3] * rhs[4] + m[7] * rhs[5] + m[11] * rhs[6] + m[15] * rhs[7],
        m[0] * rhs[8] + m[4] * rhs[9] + m[8] * rhs[10] + m[12] * rhs[11],
        m[1] * rhs[8] + m[5] * rhs[9] + m[9] * rhs[10] + m[13] * rhs[11],
        m[2] * rhs[8] + m[6] * rhs[9] + m[10] * rhs[10] + m[14] * rhs[11],
        m[3] * rhs[8] + m[7] * rhs[9] + m[11] * rhs[10] + m[15] * rhs[11],
        m[0] * rhs[12] + m[4] * rhs[13] + m[8] * rhs[14] + m[12] * rhs[15],
        m[1] * rhs[12] + m[5] * rhs[13] + m[9] * rhs[14] + m[13] * rhs[15],
        m[2] * rhs[12] + m[6] * rhs[13] + m[10] * rhs[14] + m[14] * rhs[15],
        m[3] * rhs[12] + m[7] * rhs[13] + m[11] * rhs[14] + m[15] * rhs[15]);
}

inline vec3 mat4::operator*(const vec3 &rhs) const {
    return vec3(m[0] * rhs.x + m[4] * rhs.y + m[8] * rhs.z + m[12],
                m[1] * rhs.x + m[5] * rhs.y + m[9] * rhs.z + m[13],
                m[2] * rhs.x + m[6] * rhs.y + m[10] * rhs.z + m[14]);
}

inline float mat4::operator[](int index) const { return m[index]; }

inline float &mat4::operator[](int index) { return m[index]; }

inline std::ostream &operator<<(std::ostream &os, const mat4 &m) {
    os << std::fixed << std::setprecision(6);
    os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[4] << " "
       << std::setw(10) << m[8] << " " << std::setw(10) << m[12] << "]\n"
       << "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[5] << " "
       << std::setw(10) << m[9] << " " << std::setw(10) << m[13] << "]\n"
       << "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[6] << " "
       << std::setw(10) << m[10] << " " << std::setw(10) << m[14] << "]\n"
       << "[" << std::setw(10) << m[3] << " " << std::setw(10) << m[7] << " "
       << std::setw(10) << m[11] << " " << std::setw(10) << m[15] << "]\n";
    os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
    return os;
}

struct quaternion {
    float w;       // scalar part
    float x, y, z; // vector part

    quaternion() : w(0), x(0), y(0), z(0) {}
    quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
    quaternion(const vec3 &axis, float angle);
    quaternion(const quaternion &q);

    quaternion operator*(const float a) const;
    quaternion operator*(const quaternion &rhs) const;

    // return quaternion from Euler angles
    // the rotation order is: x -> y -> z
    static quaternion make(const vec3 &angles);

    void set(float w, float x, float y, float z);
    void set(const vec3 &axis, float angle);
    quaternion &normalize();
    quaternion conjugate() const;
    quaternion inverse() const;
    mat4 to_mat4() const;

    friend std::ostream &operator<<(std::ostream &os, const quaternion &q);
};

inline quaternion::quaternion(const vec3 &axis, float angle) {
    set(axis, angle);
}

inline quaternion::quaternion(const quaternion &q) { set(q.w, q.x, q.y, q.z); }

inline quaternion quaternion::operator*(const float a) const {
    return quaternion(a * w, a * x, a * y, a * z);
}

inline quaternion quaternion::operator*(const quaternion &rhs) const {
    vec3 a(x, y, z);
    vec3 b(rhs.x, rhs.y, rhs.z);

    auto cross = a.cross(b);                // v x v'
    auto dot = a.dot(b);                    // v . v'
    auto c = cross + (w * b) + (rhs.w * a); // v x v' + sv' + s'v
    return quaternion(w * rhs.w - dot, c.x, c.y, c.z);
}

inline quaternion quaternion::make(const vec3 &angles) {
    auto x = quaternion(vec3(1, 0, 0), angles.x); // rotate along x
    auto y = quaternion(vec3(0, 1, 0), angles.y); // rotate along y
    auto z = quaternion(vec3(0, 0, 1), angles.z); // rotate along z
    return z * y * x;
}

inline void quaternion::set(float w, float x, float y, float z) {
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

inline void quaternion::set(const vec3 &axis, float angle) {
    auto v = axis;
    v.normalize();

    angle = deg2rad(angle) * 0.5f;
    float sine = sinf(angle);
    w = cosf(angle);
    x = v.x * sine;
    y = v.y * sine;
    z = v.z * sine;
}

inline quaternion &quaternion::normalize() {
    float d = w * w + x * x + y * y + z * z;
    if (d < EPSILON)
        return *this; // do nothing if it is zero
    d = 1.0f / sqrtf(d);
    w *= d;
    x *= d;
    y *= d;
    z *= d;
    return *this;
}

inline quaternion quaternion::conjugate() const {
    return quaternion(w, -x, -y, -z);
}

inline quaternion quaternion::inverse() const {
    float d = w * w + x * x + y * y + z * z;
    if (d < EPSILON)
        return *this; // do nothing if it is zero
    return conjugate() * (1.0f / d);
}

// note: assume the quaternion is unit length
inline mat4 quaternion::to_mat4() const {
    float x2 = x + x;
    float y2 = y + y;
    float z2 = z + z;
    float xx2 = x * x2;
    float xy2 = x * y2;
    float xz2 = x * z2;
    float yy2 = y * y2;
    float yz2 = y * z2;
    float zz2 = z * z2;
    float wx2 = w * x2;
    float wy2 = w * y2;
    float wz2 = w * z2;

    return mat4(1 - (yy2 + zz2), xy2 + wz2, xz2 - wy2, 0, //
                xy2 - wz2, 1 - (xx2 + zz2), yz2 + wx2, 0, //
                xz2 + wy2, yz2 - wx2, 1 - (xx2 + yy2), 0, //
                0, 0, 0, 1                                //
    );

    // for non-unit quaternion
    // ss+xx-yy-zz, 2xy+2sz,     2xz-2sy,     0
    // 2xy-2sz,     ss-xx+yy-zz, 2yz-2sx,     0
    // 2xz+2sy,     2yz+2sx,     ss-xx-yy+zz, 0
    // 0,           0,           0,           1
}

inline std::ostream &operator<<(std::ostream &os, const quaternion &q) {
    os << "quaternion(" << q.w << ", {" << q.x << ", " << q.y << ", " << q.z
       << "})";
    return os;
}
