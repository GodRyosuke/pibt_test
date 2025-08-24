#include "vec.hpp"

#include <cmath>

namespace wander_util {
Vec3::Vec3()
    : m_x(0.f), m_y(0.f), m_z(0.f)
{
}

Vec3::Vec3(float x, float y, float z)
    : m_x(x), m_y(y), m_z(z)
{
}

float Vec3::dot(const Vec3& other) const
{
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

Vec3 Vec3::cross(const Vec3& other) const
{
    return Vec3(
        m_y * other.m_z - m_z * other.m_y,
        m_z * other.m_x - m_x * other.m_z,
        m_x * other.m_y - m_y * other.m_x);
}

void Vec3::normalize()
{
    float norm = this->norm();
    m_x /= norm;
    m_y /= norm;
    m_z /= norm;
}

float Vec3::norm() const
{
    return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

double Vec3::sqNorm() const
{
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

void Vec3::operator=(const Vec3& vec)
{
    m_x = vec.m_x;
    m_y = vec.m_y;
    m_z = vec.m_z;
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return Vec3(
        m_x + other.m_x,
        m_y + other.m_y,
        m_z + other.m_z);
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return Vec3(
        m_x - other.m_x,
        m_y - other.m_y,
        m_z - other.m_z);
}

void Vec3::operator+=(const Vec3& other)
{
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
}

void Vec3::operator-=(const Vec3& other)
{
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;
}

Vec3 Vec3::operator*(float scalar) const
{
    return Vec3(
        m_x * scalar,
        m_y * scalar,
        m_z * scalar);
}

void Vec3::operator*=(float scalar)
{
    m_x *= scalar;
    m_y *= scalar;
    m_z *= scalar;
}

Vec3 Vec3::operator-() const
{
    return Vec3(-m_x, -m_y, -m_z);
}
}  // namespace wander_util
