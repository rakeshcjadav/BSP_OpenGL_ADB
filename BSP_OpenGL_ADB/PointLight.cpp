#include"pch.h"
#include"PointLight.h"
#include"Material.h"

CPointLight::CPointLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten) :
    CLight(color),
    m_vPosition(pos),
    m_vAttenuation(atten)
{

}

void CPointLight::Bind(CMaterial* pMaterial)
{
    pMaterial->SetUniform("uPointLight.position", m_vPosition);
    pMaterial->SetUniform("uPointLight.color", m_vColor);
    pMaterial->SetUniform("uPointLight.attenuation", m_vAttenuation);
}