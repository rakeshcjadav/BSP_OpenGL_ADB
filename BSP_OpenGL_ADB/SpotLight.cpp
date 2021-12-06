#include"pch.h"
#include"SpotLight.h"
#include"Material.h"

CSpotLight::CSpotLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, glm::vec3 atten, glm::vec2 cutOffAngles):
    CLight(color),
    m_vPosition(pos),
    m_vDirection(direction),
    m_vAttenuation(atten),
    m_vCutOffAngles(cutOffAngles)
{

}

void CSpotLight::Bind(CMaterial* pMaterial)
{
    pMaterial->SetUniform("uSpotLight.position", m_vPosition);
    pMaterial->SetUniform("uSpotLight.direction", m_vDirection);
    pMaterial->SetUniform("uSpotLight.color", m_vColor);
    pMaterial->SetUniform("uSpotLight.attenuation", m_vAttenuation);
    pMaterial->SetUniform("uSpotLight.cutoffAngle", m_vCutOffAngles.x);
    pMaterial->SetUniform("uSpotLight.outerCutOffAngle", m_vCutOffAngles.y);
}