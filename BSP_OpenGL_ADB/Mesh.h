#pragma once

struct SMeshData;

class CMesh
{
public:
    static CMesh* CreateRectangle();
    static CMesh* CreateRectangleTriangleList();
    static CMesh* CreateCube();
public:
    CMesh(SMeshData * pMeshData);
    CMesh(SMeshData * pMeshData, bool useIndexBuffer);
    void Render();
private:
    unsigned int m_IDMesh;
    unsigned int m_iIndexCount;
    unsigned int m_iVertexCount;
    bool m_bUseIndexBuffer;
};

struct SVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec2 colorRange;
    glm::vec3 barycentric;
    glm::vec3 displacement;

    SVertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec2 _uv, glm::vec2 _colorRange, glm::vec3 _barycentric, glm::vec3 _displacement)
    {
        position = _pos;
        normal = _normal;
        uv = _uv;
        colorRange = _colorRange;
        barycentric = _barycentric;
        displacement = _displacement;
    }

    SVertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec2 _uv, glm::vec2 _colorRange, glm::vec3 _barycentric)
    {
        position = _pos;
        normal = _normal;
        uv = _uv;
        colorRange = _colorRange;
        barycentric = _barycentric;
        displacement = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    SVertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec2 _uv, glm::vec2 _colorRange)
    {
        position = _pos;
        normal = _normal;
        uv = _uv;
        colorRange = _colorRange;
        barycentric = glm::vec3(0.0f, 0.0f, 0.0f);
        displacement = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    SVertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec2 _uv)
    {
        position = _pos;
        normal = _normal;
        uv = _uv;
        colorRange = glm::vec2(0.0f, 1.0f);
        barycentric = glm::vec3(0.0f, 0.0f, 0.0f);
        displacement = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};

struct SMeshData
{
    std::vector<SVertex> aVertices;
    std::vector<unsigned int> aIndices;
};

