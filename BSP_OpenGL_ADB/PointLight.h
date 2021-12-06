#pragma once

class CPointLight : public CLight
{
public:
    CPointLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten);

    void Bind(CMaterial* pMaterial) override;

private:
    glm::vec3 m_vPosition;
    glm::vec3 m_vAttenuation;
};