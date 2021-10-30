#pragma once
// Forward Declaration
class CShader;


class CProgram
{
public:
    CProgram(std::string strName, CShader * pVertexShader, CShader * pFragmentShader);
    unsigned int GetID();
    void Use();
    void SetUniform(std::string strName, int value);
    void SetUniform(std::string strName, float value);
    void SetUniform(std::string strName, glm::mat4 value);
private:
    std::string m_strName;
    unsigned int m_IDProgram;

};