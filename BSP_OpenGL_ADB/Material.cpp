#include"pch.h"
#include"Material.h"
#include"Program.h"
#include"Texture.h"

CMaterial::CMaterial(std::string strName, SMaterialDef* pDef, CProgram* pProgram)
{
    m_strName = strName;
    m_pMaterialDef = pDef;
    m_pProgram = pProgram;
}

CMaterial::CMaterial(std::string strName, SMaterialDef * pDef, CProgram * pProgram, std::map<std::string, CTexture*> mapTextures)
{
    m_strName = strName;
    m_pMaterialDef = pDef;
    m_pProgram = pProgram;
    m_mapTextures = mapTextures;
}

std::string CMaterial::GetName()
{
    return m_strName;
}

void CMaterial::Bind()
{
    m_pProgram->Use();

    int i = 0;
    for (auto itr = m_mapTextures.begin(); itr != m_mapTextures.end(); itr++)
    {
        std::string uniformName = itr->first;
        CTexture* pTexture = itr->second;
        if(pTexture)
            pTexture->Bind(i);
        m_pProgram->SetUniform(uniformName, i);
        i++;
    }

    if (m_pMaterialDef)
    {
        m_pProgram->SetUniform("uMaterial.material.ambient", m_pMaterialDef->ambientColor);
        m_pProgram->SetUniform("uMaterial.material.diffuse", m_pMaterialDef->diffuseColor);
        m_pProgram->SetUniform("uMaterial.material.specular", m_pMaterialDef->specularColor);
        m_pProgram->SetUniform("uMaterial.material.shininess", m_pMaterialDef->shininess);
        m_pProgram->SetUniform("uMaterial.material.specularStrength", m_pMaterialDef->specularStrength);
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