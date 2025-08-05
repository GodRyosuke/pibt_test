#include "grid.hpp"

#include <memory>
#include <vector>
#include "util/wander_math.hpp"

namespace wander_csm_test {
Grid::Grid()
{
    std::vector<wu::Vec3>     vertices;
    std::vector<unsigned int> indices;

    unsigned int index    = 0;
    float        gridSize = 100.f;
    for (float x = -gridSize; x <= gridSize; x++) {
        vertices.emplace_back(wu::Vec3(x, -gridSize, 0.f));
        vertices.emplace_back(wu::Vec3(x, gridSize, 0.f));
        indices.emplace_back(index++);
        indices.emplace_back(index++);
    }
    for (int y = -gridSize; y <= gridSize; y++) {
        vertices.emplace_back(wu::Vec3(-gridSize, y, 0.f));
        vertices.emplace_back(wu::Vec3(gridSize, y, 0.f));
        indices.emplace_back(index++);
        indices.emplace_back(index++);
    }

    std::shared_ptr<VertexArray> vao(new VertexArray(vertices, {}, {}, {}, indices));

    m_vao        = std::move(VertexArray(vertices, {}, {}, {}, indices));
    m_numIndices = index;
}

}  // namespace wander_csm_test
