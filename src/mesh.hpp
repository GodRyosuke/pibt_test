#pragma once

#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/scene.h>        // Output data structure
#include <assimp/Importer.hpp>   // C++ importer interface
#include <string>
#include <vector>
#include "util/wander_math.hpp"
#include "vertex_array.hpp"

namespace wander_csm_test {
class Mesh
{
public:
    struct BasicMeshEntry
    {
        BasicMeshEntry()
        {
            m_numIndices    = 0;
            m_baseVertex    = 0;
            m_baseIndex     = 0;
            m_materialIndex = -1;
        }

        unsigned int m_numIndices;
        unsigned int m_baseVertex;
        unsigned int m_baseIndex;
        unsigned int m_materialIndex;
    };

    Mesh(const std::string& path);
    const VertexArray&    getVAO() const { return m_vao; }
    std::size_t           getSubMeshNum() const { return m_basicMeshEntries.size(); }
    const BasicMeshEntry& getSubMeshEntry(std::size_t idx) const { return m_basicMeshEntries[idx]; }

private:
    bool load();
    bool loadMesh(const aiMesh* pMesh, unsigned int meshIdx);

    std::string      m_filePath;
    const aiScene*   m_pScene;
    Assimp::Importer m_Importer;  // Importer保持せんかったら、Sceneも保持できない!!

    std::vector<BasicMeshEntry> m_basicMeshEntries;
    std::vector<wu::Vec3>       m_positions;
    std::vector<wu::Vec3>       m_normals;
    std::vector<wu::Vec2>       m_texCoords;
    std::vector<wu::Vec3>       m_tangents;
    std::vector<unsigned int>   m_indices;
    unsigned int                m_numVertices;
    unsigned int                m_numIndices;

    VertexArray m_vao;
};
}  // namespace wander_csm_test
