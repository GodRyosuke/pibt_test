#include "grid.hpp"

#include <memory>
#include <vector>
#include "util/wander_math.hpp"

namespace wander_csm_test {
Grid::Grid()
{
    std::vector<wu::Vec3>     vertices;
    std::vector<wu::Vec3>     colors;
    std::vector<unsigned int> indices;

    unsigned int index    = 0;
    float        gridSize = 100.f;
    for (float x = -gridSize; x <= gridSize; x++) {
        if (x == 0) {
            vertices.emplace_back(wu::Vec3(x, -gridSize, 0.f));
            vertices.emplace_back(wu::Vec3(x, 0.f, 0.f));
        } else {
            vertices.emplace_back(wu::Vec3(x, -gridSize, 0.f));
            vertices.emplace_back(wu::Vec3(x, gridSize, 0.f));
        }
        colors.emplace_back(wu::Vec3(0.8, 0.8, 0.8));
        colors.emplace_back(wu::Vec3(0.8, 0.8, 0.8));
        indices.emplace_back(index++);
        indices.emplace_back(index++);
    }
    for (int y = -gridSize; y <= gridSize; y++) {
        if (y == 0) {
            vertices.emplace_back(wu::Vec3(-gridSize, y, 0.f));
            vertices.emplace_back(wu::Vec3(0.f, y, 0.f));
        } else {
            vertices.emplace_back(wu::Vec3(-gridSize, y, 0.f));
            vertices.emplace_back(wu::Vec3(gridSize, y, 0.f));
        }
        colors.emplace_back(wu::Vec3(0.8, 0.8, 0.8));
        colors.emplace_back(wu::Vec3(0.8, 0.8, 0.8));
        indices.emplace_back(index++);
        indices.emplace_back(index++);
    }

    vertices.emplace_back(wu::Vec3(0.f, 0.f, 0.f));
    vertices.emplace_back(wu::Vec3(gridSize, 0.f, 0.f));
    colors.emplace_back(wu::Vec3(1.0, 0.2, 0.2));
    colors.emplace_back(wu::Vec3(1.0, 0.2, 0.2));
    indices.emplace_back(index++);
    indices.emplace_back(index++);

    vertices.emplace_back(wu::Vec3(0.f, 0.f, 0.f));
    vertices.emplace_back(wu::Vec3(0.f, gridSize, 0.f));
    colors.emplace_back(wu::Vec3(0.2, 1.0, 0.2));
    colors.emplace_back(wu::Vec3(0.2, 1.0, 0.2));
    indices.emplace_back(index++);
    indices.emplace_back(index++);

    vertices.emplace_back(wu::Vec3(0.f, 0.f, 0.f));
    vertices.emplace_back(wu::Vec3(0.f, 0.f, gridSize));
    colors.emplace_back(wu::Vec3(0.2, 0.2, 1.0));
    colors.emplace_back(wu::Vec3(0.2, 0.2, 1.0));
    indices.emplace_back(index++);
    indices.emplace_back(index++);

    m_vao = std::move(std::unique_ptr<VertexArray>(new VertexArray(vertices, colors, {}, {}, indices)));
    m_numIndices = index;
}

}  // namespace wander_csm_test
