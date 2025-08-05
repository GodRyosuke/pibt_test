#pragma once

#include "component/component.hpp"
#include "vertex_array.hpp"

namespace wander_csm_test {
class Game;
class Shader;
class Mesh;

class MeshComponent : public Component
{
public:
    MeshComponent(Game& game, const std::string& ownerId, const Mesh& mesh);
    void draw();

private:
    VertexArray m_vao;
    Shader&     m_shader;
    const Mesh& m_mesh;
};
}  // namespace wander_csm_test
