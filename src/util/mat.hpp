#pragma once

#include <vector>

namespace wander_util {
class Vec3;

class Mat4
{
public:
    Mat4();
    Mat4(const Mat4&);
    Mat4(Mat4&&) noexcept;
    Mat4(const std::vector<double>& input);
    const float* getPtr() const { return m_data.data(); }
    float        operator()(int row, int col) const;
    Mat4&        operator=(const Mat4&);
    Mat4&        operator=(Mat4&&) noexcept;
    Mat4         operator*(const Mat4& other) const;
    void         operator=(const std::vector<double>&);
    void         transpose();
    static Mat4  identity();
    static Mat4  translation(const Vec3& trans);
    static Mat4  rotation(const Vec3& axis, double angle);
    static Mat4  scale(const Vec3& scale);
    static Mat4  view(const Vec3& pos, const Vec3& target, const Vec3& up);
    static Mat4  perspective(double fov, double aspect, double near, double far);
    static Mat4  orthogonal(double right, double left, double top, double bottom, double near, double far);

private:
    std::vector<float> m_data;
};
}  // namespace wander_util
