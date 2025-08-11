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
    virtual void draw() const;
    void         setTexture(const std::string& textureName) { m_textureName = textureName; }
    void         setColor(const wu::Vec3& color) { m_color = wu::Vec4(color, 1.0); }
    void         setColor(const wu::Vec4& color) { m_color = color; }

protected:
    const Shader& m_shader;
    const Mesh&   m_mesh;
    std::string   m_textureName;
    wu::Vec4      m_color;
};
}  // namespace wander_csm_test
