#include"pch.h"
#include"Mesh.h"


CMesh* CMesh::CreateRectangle()
{
    SMeshData data;
    
    const int subdivisionsX = 70;
    const int subdivisionsY = 70;
    const int totalVertices = (subdivisionsX + 1) * (subdivisionsY + 1);
    
    const float width = 1.0f;
    const float height = 1.0f;
    const float startX = -width / 2.0f;
    const float startY = -height / 2.0f;
    const float stepX = width / subdivisionsX;
    const float stepY = height / subdivisionsY;
    
    for (int y = 0; y <= subdivisionsY; ++y)
    {
        for (int x = 0; x <= subdivisionsX; ++x)
        {
            float posX = startX + x * stepX;
            float posY = startY + y * stepY;
            float posZ = 0.0f;
            
            glm::vec3 position(posX, posY, posZ);
            glm::vec3 normal(0.0f, 0.0f, 1.0f);
            glm::vec2 uv((float)x / subdivisionsX, (float)y / subdivisionsY);
            
            float noise = (float)(rand() % 100) / 100.0f * 0.2f - 0.1f;
            float scalarValue = sin(posX * 10.0f) * cos(posY * 10.0f) + noise;
            glm::vec2 colorRange(scalarValue, 0.0f);
            
            data.aVertices.push_back(SVertex(position, normal, uv, colorRange));
        }
    }
    
    for (int y = 0; y < subdivisionsY; ++y)
    {
        for (int x = 0; x < subdivisionsX; ++x)
        {
            int topLeft = y * (subdivisionsX + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (y + 1) * (subdivisionsX + 1) + x;
            int bottomRight = bottomLeft + 1;
            
            data.aIndices.push_back(topLeft);
            data.aIndices.push_back(bottomLeft);
            data.aIndices.push_back(topRight);
            
            data.aIndices.push_back(topRight);
            data.aIndices.push_back(bottomLeft);
            data.aIndices.push_back(bottomRight);
        }
    }

    return new CMesh(&data);
}

CMesh* CMesh::CreateRectangleTriangleList()
{
    SMeshData data;
    
    const int subdivisionsX = 70;
    const int subdivisionsY = 70;
    
    const float width = 1.0f;
    const float height = 1.0f;
    const float startX = -width / 2.0f;
    const float startY = -height / 2.0f;
    const float stepX = width / subdivisionsX;
    const float stepY = height / subdivisionsY;
    
    for (int y = 0; y < subdivisionsY; ++y)
    {
        for (int x = 0; x < subdivisionsX; ++x)
        {
            float posX0 = startX + x * stepX;
            float posX1 = startX + (x + 1) * stepX;
            float posY0 = startY + y * stepY;
            float posY1 = startY + (y + 1) * stepY;
            
            glm::vec3 normal(0.0f, 0.0f, 1.0f);
            
            glm::vec3 v0(posX0, posY0, 0.0f);
            glm::vec3 v1(posX0, posY1, 0.0f);
            glm::vec3 v2(posX1, posY1, 0.0f);
            glm::vec3 v3(posX1, posY0, 0.0f);
            
            glm::vec2 uv0((float)x / subdivisionsX, (float)y / subdivisionsY);
            glm::vec2 uv1((float)x / subdivisionsX, (float)(y + 1) / subdivisionsY);
            glm::vec2 uv2((float)(x + 1) / subdivisionsX, (float)(y + 1) / subdivisionsY);
            glm::vec2 uv3((float)(x + 1) / subdivisionsX, (float)y / subdivisionsY);
            
            float noise0 = (float)(rand() % 100) / 100.0f * 0.2f - 0.1f;
            float scalar0 = sin(v0.x * 10.0f) * cos(v0.y * 10.0f) + noise0;
            glm::vec2 color0(scalar0, 0.0f);
            
            float noise1 = (float)(rand() % 100) / 100.0f * 0.2f - 0.1f;
            float scalar1 = sin(v1.x * 10.0f) * cos(v1.y * 10.0f) + noise1;
            glm::vec2 color1(scalar1, 0.0f);
            
            float noise2 = (float)(rand() % 100) / 100.0f * 0.2f - 0.1f;
            float scalar2 = sin(v2.x * 10.0f) * cos(v2.y * 10.0f) + noise2;
            glm::vec2 color2(scalar2, 0.0f);
            
            float noise3 = (float)(rand() % 100) / 100.0f * 0.2f - 0.1f;
            float scalar3 = sin(v3.x * 10.0f) * cos(v3.y * 10.0f) + noise3;
            glm::vec2 color3(scalar3, 0.0f);
            
            glm::vec3 disp0 = glm::vec3(cos(v0.x * 5.0f) * 0.1f, sin(v0.y * 5.0f) * 0.1f, scalar0 * 0.05f);
            glm::vec3 disp1 = glm::vec3(cos(v1.x * 5.0f) * 0.1f, sin(v1.y * 5.0f) * 0.1f, scalar1 * 0.05f);
            glm::vec3 disp2 = glm::vec3(cos(v2.x * 5.0f) * 0.1f, sin(v2.y * 5.0f) * 0.1f, scalar2 * 0.05f);
            glm::vec3 disp3 = glm::vec3(cos(v3.x * 5.0f) * 0.1f, sin(v3.y * 5.0f) * 0.1f, scalar3 * 0.05f);
            
            data.aVertices.push_back(SVertex(v0, normal, uv0, color0, glm::vec3(1.0f, 0.0f, 0.0f), disp0));
            data.aVertices.push_back(SVertex(v1, normal, uv1, color1, glm::vec3(0.0f, 1.0f, 0.0f), disp1));
            data.aVertices.push_back(SVertex(v2, normal, uv2, color2, glm::vec3(0.0f, 0.0f, 1.0f), disp2));
            
            data.aVertices.push_back(SVertex(v2, normal, uv2, color2, glm::vec3(1.0f, 0.0f, 0.0f), disp2));
            data.aVertices.push_back(SVertex(v3, normal, uv3, color3, glm::vec3(0.0f, 1.0f, 0.0f), disp3));
            data.aVertices.push_back(SVertex(v0, normal, uv0, color0, glm::vec3(0.0f, 0.0f, 1.0f), disp0));
        }
    }
    
    return new CMesh(&data, false);
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
    angles.push_back(glm::vec3(180.0f, 0.0f, 0.0f));     // Back
    angles.push_back(glm::vec3(0.0f, -90.0f, 0.0f));     // Left
    angles.push_back(glm::vec3(0.0f, 90.0f, 0.0f));    // Right
    angles.push_back(glm::vec3(90.0f, 0.0f, 0.0f));      // Top
    angles.push_back(glm::vec3(-90.0f, 0.0f, 0.0f));     // bottom
    
    int face = 0;
    int vertexOffset = 4;
    for (const auto& angle : angles)
    {
        glm::mat3 mat = glm::toMat3(glm::quat(glm::radians(angle)));
        for (const SVertex& vertex : tempVertices)
        {
            glm::vec3 pos = mat * vertex.position;
            glm::vec3 normal = glm::normalize(mat * vertex.normal);
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
    m_bUseIndexBuffer = true;
    m_iIndexCount = pMeshData->aIndices.size();
    m_iVertexCount = pMeshData->aVertices.size();

    glGenVertexArrays(1, &m_IDMesh);
    glBindVertexArray(m_IDMesh);
    {
        unsigned int VBOPos;
        glGenBuffers(1, &VBOPos);
        glBindBuffer(GL_ARRAY_BUFFER, VBOPos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex)*pMeshData->aVertices.size(), pMeshData->aVertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)(2*sizeof(glm::vec3)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)(2*sizeof(glm::vec3) + sizeof(glm::vec2)));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)(2*sizeof(glm::vec3) + 2*sizeof(glm::vec2)));
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)(3*sizeof(glm::vec3) + 2*sizeof(glm::vec2)));
        glEnableVertexAttribArray(5);

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* pMeshData->aIndices.size(), pMeshData->aIndices.data(), GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

CMesh::CMesh(SMeshData* pMeshData, bool useIndexBuffer)
{
    m_bUseIndexBuffer = useIndexBuffer;
    m_iIndexCount = pMeshData->aIndices.size();
    m_iVertexCount = pMeshData->aVertices.size();

    glGenVertexArrays(1, &m_IDMesh);
    glBindVertexArray(m_IDMesh);
    {
        unsigned int VBOPos;
        glGenBuffers(1, &VBOPos);
        glBindBuffer(GL_ARRAY_BUFFER, VBOPos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex)*pMeshData->aVertices.size(), pMeshData->aVertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)(2*sizeof(glm::vec3)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)(2*sizeof(glm::vec3) + sizeof(glm::vec2)));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)(2*sizeof(glm::vec3) + 2*sizeof(glm::vec2)));
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)(3*sizeof(glm::vec3) + 2*sizeof(glm::vec2)));
        glEnableVertexAttribArray(5);

        if (useIndexBuffer && pMeshData->aIndices.size() > 0)
        {
            unsigned int EBO;
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* pMeshData->aIndices.size(), pMeshData->aIndices.data(), GL_STATIC_DRAW);
        }
    }
    glBindVertexArray(0);
}

void CMesh::Render()
{
    glBindVertexArray(m_IDMesh);
    if (m_bUseIndexBuffer)
    {
        glDrawElements(GL_TRIANGLES, m_iIndexCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, m_iVertexCount);
    }
    glBindVertexArray(0);
}