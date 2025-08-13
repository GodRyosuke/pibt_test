#pragma once

#include "mesh_component.hpp"
#include "util/wander_math.hpp"

namespace wander_csm_test {
class InstancedMeshComponent : public MeshComponent
{
public:
    struct ShaderMat4
    {
        float m11;
        float m12;
        float m13;
        float m14;
        float m21;
        float m22;
        float m23;
        float m24;
        float m31;
        float m32;
        float m33;
        float m34;
        float m41;
        float m42;
        float m43;
        float m44;
    };
    struct InstancedMeshCompData
    {
        ShaderMat4 transform;
        wu::Vec4   color;
    };

    InstancedMeshComponent(Game& game, const std::string& ownerId, const Mesh& mesh, const Shader& shader);
    InstancedMeshComponent(const InstancedMeshComponent&) = delete;
    void        draw() const override;
    void        addData(const InstancedMeshCompData& data);
    void        clearData() { m_instanceData.clear(); }
    std::size_t getNumRects() { return m_instanceData.size(); }

private:
    unsigned int                       m_shaderDataBOID;
    unsigned int                       m_instanceDataBuffSize;
    std::vector<InstancedMeshCompData> m_instanceData;
};
}  // namespace wander_csm_test