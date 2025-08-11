#include "texture.hpp"

#include <bitset>
#include <fstream>
#include <iostream>
#include "gl.hpp"
#include "stb_image.h"

namespace wander_csm_test {
Texture::Texture(const std::string& filePath)
    : m_data(0),
      m_unit(GL_TEXTURE0),
      m_width(0),
      m_height(0)
{
    // Load from file
    stbi_set_flip_vertically_on_load(true);
    m_pictureData = std::unique_ptr<unsigned char>(stbi_load(filePath.c_str(), &m_width, &m_height, &m_numChannels, 0));

    if (!m_pictureData) {
        throw std::runtime_error("failed to load texture: " + filePath);
    }

    glGenTextures(1, &m_data);
    glActiveTexture(m_unit);
    glBindTexture(GL_TEXTURE_2D, m_data);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int colorCh = 0;
    if (m_numChannels == 4) {
        colorCh = GL_RGBA;
    } else if (m_numChannels == 3) {
        colorCh = GL_RGB;
    } else if (m_numChannels == 1) {
        colorCh = GL_RED;
    } else {
        throw std::runtime_error("failed to load texture: " + filePath + ". Unsupported color ch: " + std::to_string(m_numChannels));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, colorCh, m_width, m_height, 0, colorCh, GL_UNSIGNED_BYTE, m_pictureData.get());
    glGenerateMipmap(GL_TEXTURE_2D);  // Generates MipMaps
}

Texture::Texture(const unsigned char* bitMap, int width, int height)
    : m_width(width),
      m_height(height),
      m_unit(GL_TEXTURE0)
{
    glGenTextures(1, &m_data);
    glActiveTexture(m_unit);
    glBindTexture(GL_TEXTURE_2D, m_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, bitMap);
    // Generates MipMaps
    // glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(const std::vector<unsigned char>& bitMap, int width, int height)
{
    Texture(bitMap.data(), width, height);
}

Texture::Texture(Texture&& other) noexcept
    : m_data(other.m_data),
      m_unit(other.m_unit),
      m_width(other.m_width),
      m_height(other.m_height)
{
    other.m_data   = 0;
    other.m_unit   = 0;
    other.m_width  = 0;
    other.m_height = 0;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_data);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this == &other) {
        return *this;
    }
    m_data   = other.m_data;
    m_unit   = other.m_unit;
    m_width  = other.m_width;
    m_height = other.m_height;

    other.m_data   = 0;
    other.m_unit   = 0;
    other.m_width  = 0;
    other.m_height = 0;

    return *this;
}

void Texture::bindTexture() const
{
    glActiveTexture(m_unit);
    glBindTexture(GL_TEXTURE_2D, m_data);
}

void Texture::unbindTexture() const
{
    glBindTexture(m_data, 0);
}
}  // namespace wander_csm_test
