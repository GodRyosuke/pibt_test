#pragma once

#include <vector>

namespace wander_util {
class Vec4
{
public:
    Vec4();
    Vec4(float x, float y, float z, float w);
    Vec4(class Vec3, float w);
    const float* getPtr() const { return &m_x; }

private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
};

class Vec3
{
public:
    Vec3();
    Vec3(float x, float y, float z);
    float        dot(const Vec3& other) const;
    Vec3         cross(const Vec3& other) const;
    void         normalize();
    float        norm() const;
    double       sqNorm() const;
    const float* getPtr() const { &m_x; }
    void         operator=(const Vec3& vec);
    Vec3         operator+(const Vec3& vec) const;
    void         operator+=(const Vec3& vec);
    Vec3         operator-(const Vec3& vec) const;
    void         operator-=(const Vec3& vec);
    Vec3         operator*(float scalar) const;
    void         operator*=(float scalar);
    Vec3         operator-() const;

    float&       x() { return m_x; }
    float&       y() { return m_y; }
    float&       z() { return m_z; }
    const float& x() const { return m_x; }
    const float& y() const { return m_y; }
    const float& z() const { return m_z; }

private:
    float m_x;
    float m_y;
    float m_z;
};
class Vec2
{
public:
    Vec2();
    Vec2(float x, float y);
    float  dot(const Vec2& other) const;
    void   normalize();
    float  norm() const;
    double sqNorm() const;
    Vec2   operator+(const Vec2& vec) const;
    Vec2   operator-(const Vec2& vec) const;
    Vec2   operator*(float scalar) const;
    Vec2   operator/(float scalar) const;

    float&       x() { return m_x; }
    float&       y() { return m_y; }
    const float& x() const { return m_x; }
    const float& y() const { return m_y; }

private:
    float m_x;
    float m_y;
};
}  // namespace wander_util
