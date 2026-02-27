#include"pch.h"
#include"Material.h"
#include"Program.h"

CMaterial::CMaterial(std::string strName, SMaterialDef* pDef, CProgram* pProgram)
{
    m_strName = strName;
    m_pMaterialDef = pDef;
    m_pProgram = pProgram;
}

std::string CMaterial::GetName()
{
    return m_strName;
}

void CMaterial::Bind()
{
    m_pProgram->Use();

    if (m_pMaterialDef)
    {
        SetUniform("uMaterial.ambient", m_pMaterialDef->ambientColor);
        SetUniform("uMaterial.diffuse", m_pMaterialDef->diffuseColor);
        SetUniform("uMaterial.specular", m_pMaterialDef->specularColor);
        SetUniform("uMaterial.shininess", m_pMaterialDef->shininess);
        SetUniform("uMaterial.specularStrength", m_pMaterialDef->specularStrength);
    }
}

void CMaterial::SetUniform(std::string strName, int value)
{
    m_pProgram->SetUniform(strName, value);
}

void CMaterial::SetUniform(std::string strName, float value)
{
    m_pProgram->SetUniform(strName, value);
}

void CMaterial::SetUniform(std::string strName, glm::vec2 value)
{
    m_pProgram->SetUniform(strName, value);
}

void CMaterial::SetUniform(std::string strName, glm::vec3 value)
{
    m_pProgram->SetUniform(strName, value);
}

void CMaterial::SetUniform(std::string strName, glm::mat4 value)
{
    m_pProgram->SetUniform(strName, value);
}