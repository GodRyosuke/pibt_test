#pragma once

#include "vertex_array.hpp"
#include <memory>

namespace wander_csm_test {
class Grid
{
public:
    Grid();

    const VertexArray& getVAO() const { return *m_vao; }
    std::size_t        getNumIndexSize() const { return m_numIndices; }

private:
    std::unique_ptr<VertexArray> m_vao;
    std::size_t m_numIndices;
};
}  // namespace wander_csm_test
