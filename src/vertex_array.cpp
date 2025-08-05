#include "vertex_array.hpp"
#include "gl.hpp"

namespace wander_csm_test {
VertexArray::VertexArray()
    : m_vertexArray(-1)
{
}

VertexArray::VertexArray(VertexArray&& va) noexcept
    : m_vertexArray(va.m_vertexArray),
      m_vertexBuffers(std::move(va.m_vertexBuffers))
{
    va.m_vertexArray = -1;
}

VertexArray& VertexArray::operator=(VertexArray&& va) noexcept
{
    m_vertexArray    = va.m_vertexArray;
    m_vertexBuffers  = std::move(va.m_vertexBuffers);
    va.m_vertexArray = -1;
    return *this;
}

VertexArray::VertexArray(std::vector<wu::Vec3> positions, std::vector<wu::Vec3> normals, std::vector<wu::Vec2> texcoords,
                         std::vector<wu::Vec3> tangents, std::vector<unsigned int> indices)
{
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    // Vertex Bufferの作成
    enum BUFFER_TYPE
    {
        INDEX_BUFFER,
        POS_VB,
        TEXCOORD_VB,
        NORMAL_VB,
        TANGETNT_VB,
        BONE_VB,
        NUM_BUFFER_TYPE  // required only for instancing
    };

    int numBuffers = 5;

    m_vertexBuffers.resize(numBuffers, 0);
    glGenBuffers(numBuffers, m_vertexBuffers.data());

    // Vertex Data
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal Map
    if (normals.size()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // UV
    if (texcoords.size()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0]) * texcoords.size(), &texcoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // tangents
    if (tangents.size()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[TANGETNT_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tangents[0]) * tangents.size(), &tangents[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // if (layout == PosNormTexSkin) {
    //     // Born and Weights
    //     skeleton->PopulateBuffer(m_vertexBuffers[BONE_VB]);
    // }

    // Index Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBuffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // unbind cube vertex arrays
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::~VertexArray()
{
    if (m_vertexArray == static_cast<unsigned int>(-1)) {
        return;
    }
    glDeleteBuffers(1, m_vertexBuffers.data());
    glDeleteVertexArrays(1, &m_vertexArray);
}

void VertexArray::setActive() const
{
    glBindVertexArray(m_vertexArray);
}
}  // namespace wander_csm_test
