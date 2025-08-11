#include "mesh_component.hpp"
#include "actor/camera_actor.hpp"
#include "game.hpp"
#include "mesh.hpp"
#include "shader.hpp"

namespace wander_csm_test {
MeshComponent::MeshComponent(Game& game, const std::string& ownerId, const Mesh& mesh, const Shader& shader)
    : Component(game, ownerId),
      m_shader(shader),
      m_mesh(mesh)
{
}

void MeshComponent::draw() const
{
    m_shader.useProgram();

    auto& actor = m_game.getActor<Actor>(m_ownerId);
    m_shader.setMatrix4Uniform("modelTransform", actor.getWorldTransform());

    // set camera parameters
    const auto& cameraActor = m_game.getActor<CameraActor>(m_game.getCameraId());
    m_shader.setMatrix4Uniform("cameraViewProj", cameraActor.getProjMat() * cameraActor.getViewMat());
    m_shader.setVector3Uniform("eyeWorldPos", cameraActor.getPosition());

    m_shader.setVector4Uniform("color", {0.5, 0.3, 1.0, 1.0});

    if (!m_textureName.empty()) {
        const auto& texture = m_game.loadTexture(m_textureName);
        texture.bindTexture();
    }

    m_mesh.getVAO().setActive();
    for (std::size_t subMeshIdx = 0; subMeshIdx < m_mesh.getSubMeshNum(); subMeshIdx++) {
        const auto& subMesh = m_mesh.getSubMeshEntry(subMeshIdx);
        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 subMesh.m_numIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * subMesh.m_baseIndex),
                                 subMesh.m_baseVertex);
    }
}
}  // namespace wander_csm_test
