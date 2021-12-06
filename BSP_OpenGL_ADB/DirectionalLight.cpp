#include"pch.h"
#include"DirectionalLight.h"
#include"Material.h"

CDirectionalLight::CDirectionalLight(glm::vec3 direction, glm::vec3 color):
    CLight(color),
    m_vDirection(direction)
{

}

void CDirectionalLight::Bind(CMaterial* pMaterial)
{
    pMaterial->SetUniform("uDirectionalLight.direction", m_vDirection);
    pMaterial->SetUniform("uDirectionalLight.color", m_vColor);
}