#pragma once

class CProgram;
struct SMaterialDef;

class CMaterial
{
public:
    CMaterial(std::string strName, SMaterialDef* pDef, CProgram* pProgram);

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
};

struct SMaterialDef
{
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
    float specularStrength;
};