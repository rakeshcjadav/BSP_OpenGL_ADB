#pragma once

class CProgram;
class CTexture;
struct SMaterialDef;

class CMaterial
{
public:
    CMaterial(std::string strName, SMaterialDef* pDef, CProgram* pProgram);
    CMaterial(std::string strName, SMaterialDef * pDef, CProgram * pProgram, std::map<std::string, CTexture *> mapTextures);

    std::string GetName();
    void Bind();
    void SetUniform(std::string strName, int value);
    void SetUniform(std::string strName, float value);
    void SetUniform(std::string strName, glm::vec2 value);
    void SetUniform(std::string strName, glm::vec3 value);
    void SetUniform(std::string strName, glm::mat4 value);

private:
    std::string m_strName;
    SMaterialDef* m_pMaterialDef;
    CProgram* m_pProgram;
    std::map<std::string, CTexture*> m_mapTextures;
};

struct SMaterialDef
{
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
    float specularStrength;
};