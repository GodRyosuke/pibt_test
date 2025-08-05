#include <algorithm>
#include <cassert>
#include <cmath>
#include "mat.hpp"
#include "vec.hpp"

namespace wander_util {
Mat4::Mat4()
    : m_data(16, 0.f)
{
}

Mat4::Mat4(const Mat4& input)
{
    m_data = input.m_data;
}

Mat4::Mat4(Mat4&& input) noexcept
{
    m_data = std::move(input.m_data);
}

Mat4& Mat4::operator=(const Mat4& input)
{
    Mat4 result;
    result.m_data = input.m_data;
    return result;
}

Mat4& Mat4::operator=(Mat4&& input) noexcept
{
    if (this == &input) {
        return *this;
    }

    m_data = std::move(input.m_data);
    return *this;
}

Mat4::Mat4(const std::vector<double>& input)
{
    assert(input.size() == 16);
    m_data.resize(16);
    std::transform(input.begin(), input.end(), m_data.begin(), [](double value) {
        return static_cast<float>(value);
    });
}

Mat4 Mat4::operator*(const Mat4& rhs) const
{
    Mat4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.m_data[i * 4 + j] += m_data[i * 4 + k] * rhs.m_data[k * 4 + j];
            }
        }
    }

    return result;
}

void Mat4::operator=(const std::vector<double>& input)
{
    assert(input.size() == 16);
    std::transform(input.begin(), input.end(), m_data.begin(), [](double value) {
        return static_cast<float>(value);
    });
}

void Mat4::transpose()
{
    std::vector<float> result(16, 0.f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[j * 4 + i] = m_data[i * 4 + j];
        }
    }
    m_data = result;
}

Mat4 Mat4::identity()
{
    return Mat4({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
}

Mat4 Mat4::translation(const Vec3& translation)
{
    return Mat4({1, 0, 0, translation.x(), 0, 1, 0, translation.y(), 0, 0, 1, translation.z(), 0, 0, 0, 1});
}

Mat4 Mat4::rotation(const Vec3& axis, double theta)
{
    double ux       = axis.x();
    double uy       = axis.y();
    double uz       = axis.z();
    double cosTheta = std::cos(theta);
    double sinTheta = std::sin(theta);

    return Mat4({cosTheta + ux * ux * (1 - cosTheta), ux * uy * (1 - cosTheta) - uz * sinTheta, ux * uz * (1 - cosTheta) + uy * sinTheta, 0,
                 uy * ux * (1 - cosTheta) + uz * sinTheta, cosTheta + uy * uy * (1 - cosTheta), uy * uz * (1 - cosTheta) - ux * sinTheta, 0,
                 uz * ux * (1 - cosTheta) - uy * sinTheta, uz * uy * (1 - cosTheta) + ux * sinTheta, cosTheta + uz * uz * (1 - cosTheta), 0,
                 0, 0, 0, 1});
}

Mat4 Mat4::scale(const Vec3& scale)
{
    return Mat4({scale.x(), 0, 0, 0, 0, scale.y(), 0, 0, 0, 0, scale.z(), 0, 0, 0, 0, 1});
}

Mat4 Mat4::view(const Vec3& pos, const Vec3& target, const Vec3& up)
{
    Vec3 clipZ = pos - target;
    clipZ.normalize();
    Vec3 clipX = up.cross(clipZ);
    clipX.normalize();
    Vec3 clipY = clipZ.cross(clipX);

    Vec3 viewTranslation = {-clipX.dot(pos), -clipY.dot(pos), -clipZ.dot(pos)};
    return Mat4({clipX.x(), clipX.y(), clipX.z(), viewTranslation.x(),
                 clipY.x(), clipY.y(), clipY.z(), viewTranslation.y(),
                 clipZ.x(), clipZ.y(), clipZ.z(), viewTranslation.z(),
                 0.0, 0.0, 0.0, 1.0});
}

Mat4 Mat4::perspective(double fov, double aspect, double near, double far)
{
    return Mat4({1.f / std::tan(fov / 2), 0.0, 0.0, 0.0,
                 0.0, 1.f / (aspect * std::tan(fov / 2)), 0.0, 0.0,
                 0.0, 0.0, -(far + near) / (far - near), -2 * far * near / (far - near),
                 0.0, 0.0, -1.0, 0.0});
}

Mat4 Mat4::orthogonal(double right, double left, double top, double bottom, double near, double far)
{
    return Mat4({2 / (right - left), 0, 0, -(right + left) / (right - left),
                 0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
                 0, 0, -2 / (far - near), -(far + near) / (far - near),
                 0, 0, 0, 1});
}
}  // namespace wander_util
