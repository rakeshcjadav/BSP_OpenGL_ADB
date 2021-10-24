#pragma once
#include<string>

class CShader
{
public:
    enum class SHADER_TYPE
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER
    };
    CShader(SHADER_TYPE shaderType, std::string strShaderSource);
    ~CShader();
    unsigned int GetID();
private:
    unsigned int m_IDShader;
};