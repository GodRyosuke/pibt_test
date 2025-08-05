#include "mesh_component.hpp"
#include "actor/camera_actor.hpp"
#include "game.hpp"
#include "mesh.hpp"
#include "shader.hpp"

namespace wander_csm_test {
MeshComponent::MeshComponent(Game& game, const std::string& ownerId, const Mesh& mesh)
    : Component(game, ownerId),
      m_shader(game.getShader("testMeshShader")),
      m_mesh(mesh)
{
}

void MeshComponent::draw()
{
    m_shader.useProgram();

    // set test cube model transforms
    // wu::Mat4 translation = std::move(wu::Mat4::translation({0.f, 0.f, 0.5f}));
    // wu::Mat4 rotation    = std::move(wu::Mat4::rotation(wu::Vec3(0.f, 0.f, 1.f), 1.5));
    // wu::Mat4 scale       = std::move(wu::Mat4::scale({0.5f, 0.5f, 0.5f}));
    auto& actor = m_game.getActor<Actor>(m_ownerId);
    m_shader.setMatrix4Uniform("modelTransform", actor.getWorldTransform());
    // m_shader.setMatrix4Uniform("modelTransform", translation * rotation * scale);

    // set camera parameters
    const auto& cameraActor = m_game.getActor<CameraActor>(m_game.getCameraId());
    m_shader.setMatrix4Uniform("cameraViewProj", cameraActor.getProjMat() * cameraActor.getViewMat());
    m_shader.setVector3Uniform("eyeWorldPos", cameraActor.getPosition());

    m_shader.setVector4Uniform("color", {0.5, 0.3, 1.0, 1.0});

    // const auto& mesh = m_meshes["test_cube"];
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
