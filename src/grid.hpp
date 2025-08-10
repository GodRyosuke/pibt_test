#pragma once

#include "vertex_array.hpp"

namespace wander_csm_test {
class Grid
{
public:
    Grid();

    const VertexArray& getVAO() const { return m_vao; }
    std::size_t        getNumIndexSize() const { return m_numIndices; }

private:
    VertexArray m_vao;
    std::size_t m_numIndices;
};
}  // namespace wander_csm_test
