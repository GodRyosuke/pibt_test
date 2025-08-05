#pragma once

#include <vector>
#include "util/wander_math.hpp"

namespace wander_csm_test {
class VertexArray
{
public:
    VertexArray();
    VertexArray(std::vector<wu::Vec3> positions, std::vector<wu::Vec3> normals, std::vector<wu::Vec2> texcoords,
                std::vector<wu::Vec3> tangents, std::vector<unsigned int> indices);

    VertexArray(const VertexArray&)            = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&&) noexcept;
    VertexArray& operator=(VertexArray&&) noexcept;
    ~VertexArray();
    void setActive() const;

private:
    unsigned int              m_vertexArray;
    std::vector<unsigned int> m_vertexBuffers;
};
}  // namespace wander_csm_test
