#pragma once

#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION

namespace wander_csm_test {
class Texture
{
public:
    Texture(const std::string& filePath);
    Texture(const unsigned char* bitMap, int width, int height);
    Texture(const std::vector<unsigned char>& bitMap, int width, int height);
    Texture(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& other) noexcept;

    void bindTexture() const;
    void unbindTexture() const;
    int  getWidth() const { return m_width; }
    int  getHeight() const { return m_height; }

private:
    unsigned int m_data;
    unsigned int m_unit;
    int          m_width;
    int          m_height;
};
}  // namespace wander_csm_test
