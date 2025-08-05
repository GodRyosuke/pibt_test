#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include "gl.hpp"

namespace wander_csm_test {
Shader::Shader()
    : m_shaderProgram(-1)
{
}

Shader::~Shader()
{
    glDeleteProgram(m_shaderProgram);
}

static bool compileShader(const std::string& filepath, GLenum shaderType, GLuint& shader)
{
    std::ifstream shaderFile(filepath);
    if (shaderFile.is_open()) {
        // Read all the text into a string
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        std::string contents     = sstream.str();
        const char* contentsChar = contents.c_str();

        // Create a shader of the specified type
        shader = glCreateShader(shaderType);
        // Set the source characters and try to compile
        glShaderSource(shader, 1, &(contentsChar), nullptr);
        glCompileShader(shader);

        GLint status;
        // Query the compile status
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE) {
            char buffer[512];
            memset(buffer, 0, 512);
            glGetShaderInfoLog(shader, 511, nullptr, buffer);
            printf("GLSL Compile Failed:\n%s", buffer);
            return false;
        }
    } else {
        printf("Shader file not found: %s\n", filepath.c_str());
        return false;
    }
    return true;
}

bool Shader::createShaderProgram(const std::string& vertFilePath, const std::string& fragFilePath)
{
    GLuint VertexShader;
    GLuint FragmentShader;
    // Compile vertex and pixel shaders
    if (!compileShader(vertFilePath,
                       GL_VERTEX_SHADER,
                       VertexShader) ||
        !compileShader(fragFilePath,
                       GL_FRAGMENT_SHADER,
                       FragmentShader)) {
        return false;
    }

    // Now create a shader program that
    // links together the vertex/frag shaders
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, VertexShader);
    glAttachShader(m_shaderProgram, FragmentShader);
    glLinkProgram(m_shaderProgram);

    // Verify that the program linked successfully
    GLint status;
    // Query the link status
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetProgramInfoLog(m_shaderProgram, 511, nullptr, buffer);
        std::string err_msg = buffer;
        std::cout << "GLSL Link Status: " << err_msg << std::endl;
        return false;
    }

    glUseProgram(m_shaderProgram);
    return true;
}

void Shader::useProgram() const
{
    glUseProgram(m_shaderProgram);
}

// void Shader::setMatrix3Uniform(const std::string& uniformName, const arma::mat& mat) const
// {
//     GLuint loc = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
//     glUniformMatrix3fv(loc, 1, GL_FALSE, mat.memptr());
// }

void Shader::setMatrix4Uniform(const std::string& uniformName, const wu::Mat4& mat) const
{
    // glUniformMatrix4fvじゃないと値設定されない説
    // GL_ARB_gpu_shader_fp64 extensionがないと正常に動かない？？
    GLuint loc = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
    glUniformMatrix4fv(loc, 1, GL_TRUE, mat.getPtr());
}

void Shader::setVector2Uniform(const std::string& uniformName, const wu::Vec2& vec) const
{
    GLuint loc = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
    glUniform2fv(loc, 1, (const GLfloat*)&vec);
}

void Shader::setVector3Uniform(const std::string& uniformName, const wu::Vec3& vec) const
{
    GLuint loc = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
    glUniform3fv(loc, 1, (const GLfloat*)&vec);
}

void Shader::setVector4Uniform(const std::string& uniformName, const wu::Vec4& vec) const
{
    GLuint loc = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
    glUniform4fv(loc, 1, vec.getPtr());
}

void Shader::setFloatUniform(const std::string& uniformName, double data) const
{
    glUniform1d(glGetUniformLocation(m_shaderProgram, uniformName.c_str()), data);
}

void Shader::setSamplerUniform(const std::string& uniformName, unsigned int tex) const
{
    glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName.c_str()), tex);
}
}  // namespace wander_csm_test
