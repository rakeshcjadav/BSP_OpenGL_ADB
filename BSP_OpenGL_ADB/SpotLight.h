#pragma once

class CSpotLight : public CLight
{
public:
    CSpotLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, glm::vec3 atten, glm::vec2 cutOffAngles);

    void Bind(CMaterial* pMaterial) override;

private:
    glm::vec3 m_vPosition;
    glm::vec3 m_vDirection;
    glm::vec3 m_vAttenuation;
    glm::vec2 m_vCutOffAngles;
};