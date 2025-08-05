#include "mesh.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include "util/wander_math.hpp"

namespace wander_csm_test {
Mesh::Mesh(const std::string& path)
    : m_filePath(path),
      m_numVertices(0),
      m_numIndices(0)
{
    if (!load()) {
        throw std::runtime_error("Failed to load mesh");
    }
}

bool Mesh::load()
{
    std::cout << "start load mesh: " << m_filePath << std::endl;
    m_pScene = m_Importer.ReadFile(m_filePath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!m_pScene) {
        printf("Error parsing '%s': '%s'\n", m_filePath.c_str(), m_Importer.GetErrorString());
        return false;
    }

    m_basicMeshEntries.resize(m_pScene->mNumMeshes);

    for (unsigned int i = 0; i < m_basicMeshEntries.size(); i++) {
        m_basicMeshEntries[i].m_materialIndex = m_pScene->mMeshes[i]->mMaterialIndex;  // MeshとMaterialの紐づけ
        m_basicMeshEntries[i].m_numIndices    = m_pScene->mMeshes[i]->mNumFaces * 3;
        m_basicMeshEntries[i].m_baseVertex    = m_numVertices;
        m_basicMeshEntries[i].m_baseIndex     = m_numIndices;

        m_numVertices += m_pScene->mMeshes[i]->mNumVertices;
        m_numIndices += m_basicMeshEntries[i].m_numIndices;
    }

    // ReserveVertexSpace();
    m_positions.reserve(m_numVertices);
    m_normals.reserve(m_numVertices);
    m_texCoords.reserve(m_numVertices);
    m_indices.reserve(m_numIndices);

    // Mesh(頂点情報など)の読み込み
    for (int meshIdx = 0; meshIdx < m_basicMeshEntries.size(); meshIdx++) {
        const aiMesh* paiMesh = m_pScene->mMeshes[meshIdx];
        printf("mesh name: %s, mesh idx: %d\n", paiMesh->mName.C_Str(), meshIdx);
        if (!loadMesh(paiMesh, meshIdx)) {
            return false;
        }
    }

    // VAO
    m_vao = VertexArray(m_positions, m_normals, m_texCoords, m_tangents, m_indices);

    m_normals.clear();
    m_texCoords.clear();

    return true;
}

bool Mesh::loadMesh(const aiMesh* pMesh, unsigned int meshIdx)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Vertex, Normal, UV取得
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
        const aiVector3D& pPos = pMesh->mVertices[i];
        m_positions.emplace_back(wu::Vec3({pPos.x, pPos.y, pPos.z}));

        if (pMesh->mNormals) {
            const aiVector3D& pNormal = pMesh->mNormals[i];
            m_normals.emplace_back(wu::Vec3({pNormal.x, pNormal.y, pNormal.z}));
        } else {
            aiVector3D Normal(0.0f, 1.0f, 0.0f);
            m_normals.emplace_back(wu::Vec3({Normal.x, Normal.y, Normal.z}));
        }

        const aiVector3D& pTexCoord = pMesh->HasTextureCoords(0) ? pMesh->mTextureCoords[0][i] : Zero3D;
        m_texCoords.emplace_back(wu::Vec2({pTexCoord.x, pTexCoord.y}));
    }

    // Index 情報取得
    for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
        const aiFace& Face = pMesh->mFaces[i];
        if (Face.mNumIndices != 3) {
            std::cout << "error: num face indices is not 3" << std::endl;
            return false;
        }
        m_indices.emplace_back(Face.mIndices[0]);
        m_indices.emplace_back(Face.mIndices[1]);
        m_indices.emplace_back(Face.mIndices[2]);
    }

    // Tangent 情報取得
    m_tangents.resize(m_positions.size());
    for (int i = 0; i < m_indices.size(); i += 3) {
        wu::Vec3  v0   = m_positions[m_indices[i]];
        wu::Vec3  v1   = m_positions[m_indices[i + 1]];
        wu::Vec3  v2   = m_positions[m_indices[i + 2]];
        wu::Vec3& tan0 = m_tangents[m_indices[i]];
        wu::Vec3& tan1 = m_tangents[m_indices[i + 1]];
        wu::Vec3& tan2 = m_tangents[m_indices[i + 2]];
        wu::Vec2  uv0  = m_texCoords[m_indices[i]];
        wu::Vec2  uv1  = m_texCoords[m_indices[i + 1]];
        wu::Vec2  uv2  = m_texCoords[m_indices[i + 2]];

        wu::Vec3 Edge1 = v1 - v0;
        wu::Vec3 Edge2 = v2 - v0;

        float DeltaU1 = uv1.x() - uv0.x();
        float DeltaV1 = uv1.y() - uv0.y();
        float DeltaU2 = uv2.x() - uv0.x();
        float DeltaV2 = uv2.y() - uv0.y();

        float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

        wu::Vec3 Tangent, Bitangent;

        Tangent.x() = f * (DeltaV2 * Edge1.x() - DeltaV1 * Edge2.x());
        Tangent.y() = f * (DeltaV2 * Edge1.y() - DeltaV1 * Edge2.y());
        Tangent.z() = f * (DeltaV2 * Edge1.z() - DeltaV1 * Edge2.z());

        Bitangent.x() = f * (-DeltaU2 * Edge1.x() + DeltaU1 * Edge2.x());
        Bitangent.y() = f * (-DeltaU2 * Edge1.y() + DeltaU1 * Edge2.y());
        Bitangent.z() = f * (-DeltaU2 * Edge1.z() + DeltaU1 * Edge2.z());

        tan0 += Tangent;
        tan1 += Tangent;
        tan2 += Tangent;
    }

    for (int i = 0; i < m_tangents.size(); i++) {
        m_tangents[i].normalize();
    }

    return true;
}
}  // namespace wander_csm_test
