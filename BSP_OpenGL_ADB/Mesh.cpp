#include"pch.h"
#include"Mesh.h"


CMesh* CMesh::CreateRectangle()
{
    SMeshData data;
    data.aVertices.push_back(SVertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
    data.aVertices.push_back(SVertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
    data.aVertices.push_back(SVertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    data.aVertices.push_back(SVertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)));

    data.aIndices = { 0, 1, 2, 0, 2, 3 };

    return new CMesh(&data);
}

CMesh* CMesh::CreateCube()
{
    SMeshData data;

    std::vector<SVertex> tempVertices;
    tempVertices.push_back(SVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
    tempVertices.push_back(SVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
    tempVertices.push_back(SVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    tempVertices.push_back(SVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
    
    std::vector<glm::vec3> angles;
    angles.push_back(glm::vec3(0.0f, 0.0f, 0.0f));      // Front
    angles.push_back(glm::vec3(0.0f, 180.0f, 0.0f));      // Back
    angles.push_back(glm::vec3(0.0f, 90.0f, 0.0f));     // Left
    angles.push_back(glm::vec3(0.0f, -90.0f, 0.0f));    // Right
    angles.push_back(glm::vec3(90.0f, 0.0f, 0.0f));      // Top
    angles.push_back(glm::vec3(-90.0f, 0.0f, 0.0f));      // bottom
    
    int face = 0;
    int vertexOffset = 4;
    for (const auto& angle : angles)
    {
        glm::mat3 mat = glm::toMat3(glm::quat(glm::radians(angle)));
        for (const SVertex& vertex : tempVertices)
        {
            glm::vec3 pos = mat * vertex.position;
            glm::vec3 normal = mat * vertex.normal;
            data.aVertices.push_back(SVertex(pos, normal, vertex.uv));
        }
        data.aIndices.push_back(face * vertexOffset);
        data.aIndices.push_back(face * vertexOffset + 1);
        data.aIndices.push_back(face * vertexOffset + 2);

        data.aIndices.push_back(face * vertexOffset);
        data.aIndices.push_back(face * vertexOffset + 2);
        data.aIndices.push_back(face * vertexOffset + 3);
        face++;
    }

    return new CMesh(&data);
}


CMesh::CMesh(SMeshData* pMeshData)
{
    float colors[] = {
        //  (r, g, b)
       1.0f, 1.0f, 1.0f,     // bottom left
       1.0f, 1.0f, 1.0f,     // bottom right
       1.0f, 1.0f, 1.0f,     // top right
       1.0f, 1.0f, 1.0f,     // top left
    };

    m_iIndexCount = pMeshData->aIndices.size();

    glGenVertexArrays(1, &m_IDMesh);
    glBindVertexArray(m_IDMesh);
    {
        unsigned int VBOPos;
        glGenBuffers(1, &VBOPos);
        glBindBuffer(GL_ARRAY_BUFFER, VBOPos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex)*pMeshData->aVertices.size(), pMeshData->aVertices.data(), GL_STATIC_DRAW);

        // Position
        // at location 0, read 3 floats and jump (stride) 4 floats for next vertex.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(1);

        // UV
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)(2*sizeof(glm::vec3)));
        glEnableVertexAttribArray(2);

        /*
        unsigned int VBOCol;
        glGenBuffers(1, &VBOCol);
        glBindBuffer(GL_ARRAY_BUFFER, VBOCol);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        */

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* pMeshData->aIndices.size(), pMeshData->aIndices.data(), GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

void CMesh::Render()
{
    glBindVertexArray(m_IDMesh);
    glDrawElements(GL_TRIANGLES, m_iIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}