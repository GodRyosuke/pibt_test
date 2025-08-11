#include "instanced_mesh_component.hpp"
#include <cassert>
#include "actor/actor.hpp"
#include "actor/camera_actor.hpp"
#include "game.hpp"
#include "gl.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "util/wander_math.hpp"

namespace wander_csm_test {
InstancedMeshComponent::InstancedMeshComponent(Game& game, const std::string& ownerId, const Mesh& mesh, const Shader& shader)
    : MeshComponent(game, ownerId, mesh, shader),
      m_instanceDataBuffSize(512)
{
    m_instanceData.reserve(m_instanceDataBuffSize);
    glGenBuffers(1, &m_shaderDataBOID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_shaderDataBOID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(InstancedMeshCompData) * m_instanceDataBuffSize,
                 m_instanceData.data(), GL_DYNAMIC_DRAW);
}

void InstancedMeshComponent::draw() const
{
    m_shader.useProgram();

    auto& actor = m_game.getActor<Actor>(m_ownerId);

    // set camera parameters
    const auto& cameraActor = m_game.getActor<CameraActor>(m_game.getCameraId());
    m_shader.setMatrix4Uniform("cameraViewProj", cameraActor.getProjMat() * cameraActor.getViewMat());
    m_shader.setVector3Uniform("eyeWorldPos", cameraActor.getPosition());

    if (!m_textureName.empty()) {
        const auto& texture = m_game.loadTexture(m_textureName);
        texture.bindTexture();
    }

    m_mesh.getVAO().setActive();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_shaderDataBOID);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(InstancedMeshCompData) * m_instanceData.size(),
                    m_instanceData.data());

    for (std::size_t subMeshIdx = 0; subMeshIdx < m_mesh.getSubMeshNum(); subMeshIdx++) {
        const auto& subMesh = m_mesh.getSubMeshEntry(subMeshIdx);
        glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
                                          subMesh.m_numIndices,
                                          GL_UNSIGNED_INT,
                                          (void*)(sizeof(unsigned int) * subMesh.m_baseIndex),
                                          m_instanceData.size(),
                                          subMesh.m_baseVertex);
    }
}

void InstancedMeshComponent::addData(const InstancedMeshCompData& data)
{
    m_instanceData.emplace_back(data);
    if (m_instanceDataBuffSize <= m_instanceData.size()) {
        if (m_instanceDataBuffSize == 0) {
            m_instanceDataBuffSize = 1;
        } else {
            m_instanceDataBuffSize *= 2;
        }
        GLuint newBOID;
        glGenBuffers(1, &newBOID);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, newBOID);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(InstancedMeshCompData) * m_instanceDataBuffSize, nullptr, GL_DYNAMIC_DRAW);
        glDeleteBuffers(1, &m_shaderDataBOID);
        m_shaderDataBOID = newBOID;
    }
}
}  // namespace wander_csm_test
