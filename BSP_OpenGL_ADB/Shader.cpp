#include"pch.h"
#include"Shader.h"

CShader::CShader(SHADER_TYPE shaderType, std::string strShaderSource)
{
    if (shaderType == SHADER_TYPE::VERTEX_SHADER)
    {
        m_IDShader = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (shaderType == SHADER_TYPE::FRAGMENT_SHADER)
    {
        m_IDShader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else if (shaderType == SHADER_TYPE::COMPUTE_SHADER)
    {
        m_IDShader = glCreateShader(GL_COMPUTE_SHADER);
    }

    const char* shaderSource = strShaderSource.c_str();
    glShaderSource(m_IDShader, 1, &shaderSource, NULL);
    glCompileShader(m_IDShader);

    int  success;
    glGetShaderiv(m_IDShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(m_IDShader, 512, NULL, infoLog);
        if (shaderType == SHADER_TYPE::VERTEX_SHADER)
        {
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        else if (shaderType == SHADER_TYPE::FRAGMENT_SHADER)
        {
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        else if (shaderType == SHADER_TYPE::COMPUTE_SHADER)
        {
            std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
}

CShader::~CShader()
{
    glDeleteShader(m_IDShader);
}

unsigned int CShader::GetID()
{
    return m_IDShader;
}