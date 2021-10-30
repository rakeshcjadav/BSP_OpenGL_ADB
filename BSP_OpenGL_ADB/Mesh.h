#pragma once

struct SMeshData;

class CMesh
{
public:
    static CMesh* CreateRectangle();
    static CMesh* CreateCube();
public:
    CMesh(SMeshData * pMeshData);
    void Render();
private:
    unsigned int m_IDMesh;
    unsigned int m_iIndexCount;
};

struct SVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    SVertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec2 _uv)
    {
        position = _pos;
        normal = _normal;
        uv = _uv;
    }
};

struct SMeshData
{
    std::vector<SVertex> aVertices;
    std::vector<unsigned int> aIndices;
};

