#pragma once

#include "gl.hpp"

namespace wander_csm_test {
class TextureShadowMap
{
public:
    TextureShadowMap(int width, int height);
    void bind(GLenum slot) const;
    void writeBind() const;

private:
    int          m_width;
    int          m_height;
    unsigned int m_fbo;
    unsigned int m_shadowMap;
};
}  // namespace wander_csm_test
