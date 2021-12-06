#pragma once

class CMaterial;

class CLight
{
public:
    virtual void Bind(CMaterial* pMaterial) = 0;

protected:
    CLight(glm::vec3 color);
    virtual ~CLight();

protected:
    glm::vec3 m_vColor;
};