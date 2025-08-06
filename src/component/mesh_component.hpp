#pragma once

#include "component/component.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

namespace wander_csm_test {
class Game;
class Shader;
class Mesh;

class MeshComponent : public Component
{
public:
    MeshComponent(Game& game, const std::string& ownerId, const Mesh& mesh, const Shader& shader);
    void draw();
    void setTexture(const std::string& textureName) { m_textureName = textureName; }

private:
    const Shader& m_shader;
    const Mesh&   m_mesh;
    std::string   m_textureName;
};
}  // namespace wander_csm_test
