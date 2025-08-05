#include "sprite_mesh.hpp"
#include <vector>
#include "util/wander_math.hpp"

namespace wander_csm_test {
SpriteMesh::SpriteMesh()
{
    std::vector<wu::Vec3> vertices = {
        wu::Vec3({-0.5f, 0.5f, 0.f}),  // top left
        wu::Vec3({0.5f, 0.5f, 0.f}),   // top right
        wu::Vec3({0.5f, -0.5f, 0.f}),  // bottom right
        wu::Vec3({-0.5f, -0.5f, 0.f})  // bottom left
    };
    std::vector<wu::Vec2> texcoords = {
        wu::Vec2({0.f, 1.f}),  // bottom left
        wu::Vec2({1.f, 1.f}),  // bottom right
        wu::Vec2({1.f, 0.f}),  // top right
        wu::Vec2({0.f, 0.f})   // top left
    };
    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0};

    m_vao = std::move(VertexArray(vertices, {}, texcoords, {}, indices));
}
}  // namespace wander_csm_test
