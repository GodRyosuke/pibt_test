#include "vec.hpp"

namespace wander_util {
Vec4::Vec4()
    : m_x(0.f), m_y(0.f), m_z(0.f), m_w(0.f)
{
}

Vec4::Vec4(float x, float y, float z, float w)
    : m_x(x), m_y(y), m_z(z), m_w(w)
{
}

Vec4::Vec4(Vec3 vec, float w)
    : m_x(vec.x()), m_y(vec.y()), m_z(vec.z()), m_w(w)
{
}
}  // namespace wander_util
