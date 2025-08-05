#pragma once

#include <string>
#include "gl.hpp"
#include "util/wander_math.hpp"

namespace wander_csm_test {
class Shader
{
public:
    Shader();
    ~Shader();

    bool createShaderProgram(const std::string& vertFilePath, const std::string& fragFilePath);
    void useProgram() const;
    // void setMatrix3Uniform(const std::string& uniformName, const wu::Mat3& mat) const;
    void setMatrix4Uniform(const std::string& uniformName, const wu::Mat4& mat) const;
    void setVector2Uniform(const std::string& uniformName, const wu::Vec2& vec) const;
    void setVector3Uniform(const std::string& uniformName, const wu::Vec3& vec) const;
    void setVector4Uniform(const std::string& uniformName, const wu::Vec4& vec) const;
    void setFloatUniform(const std::string& uniformName, double data) const;
    void setSamplerUniform(const std::string& uniformName, unsigned int tex) const;

    unsigned int m_shaderProgram;
    // private:
};
}  // namespace wander_csm_test
