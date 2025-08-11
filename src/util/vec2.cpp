#include "vec.hpp"

#include <cmath>

namespace wander_util {
Vec2::Vec2()
    : m_x(0.f), m_y(0.f)
{
}

Vec2::Vec2(float x, float y)
    : m_x(x), m_y(y)
{
}

float Vec2::dot(const Vec2& other) const
{
    return m_x * other.m_x + m_y * other.m_y;
}

void Vec2::normalize()
{
    float norm = this->norm();
    m_x /= norm;
    m_y /= norm;
}

float Vec2::norm() const
{
    return std::sqrt(m_x * m_x + m_y * m_y);
}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2(m_x + other.m_x, m_y + other.m_y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return Vec2(m_x - other.m_x, m_y - other.m_y);
}

Vec2 Vec2::operator*(float scalar) const
{
    return Vec2(m_x * scalar, m_y * scalar);
}

Vec2 Vec2::operator/(float scalar) const
{
    return Vec2(m_x / scalar, m_y / scalar);
}
}  // namespace wander_util
