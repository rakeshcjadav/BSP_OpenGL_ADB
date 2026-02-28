#include"pch.h"
#include"Program.h"
#include"Shader.h"

CProgram::CProgram(std::string strName, CShader* pVertexShader, CShader* pFragmentShader)
{
    m_strName = strName;

    m_IDProgram = glCreateProgram();
    glAttachShader(m_IDProgram, pVertexShader->GetID());
    glAttachShader(m_IDProgram, pFragmentShader->GetID());
    glLinkProgram(m_IDProgram);

    int success;
    glGetProgramiv(m_IDProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_IDProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

CProgram::CProgram(std::string strName, CShader* pComputeShader)
{
    m_strName = strName;

    m_IDProgram = glCreateProgram();
    glAttachShader(m_IDProgram, pComputeShader->GetID());
    glLinkProgram(m_IDProgram);

    int success;
    glGetProgramiv(m_IDProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_IDProgram, 512, NULL, infoLog);
        std::cout << "ERROR::COMPUTE_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

unsigned int CProgram::GetID()
{
    return m_IDProgram;
}

void CProgram::Use()
{
    glUseProgram(m_IDProgram);
    SetUniform("uMaterial.DiffuseMapTiling", glm::vec2(1.0f, 1.0f));
}

void CProgram::Dispatch(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ)
{
    glUseProgram(m_IDProgram);
    glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void CProgram::SetUniform(std::string strName, int value)
{
    glUniform1i(glGetUniformLocation(m_IDProgram, strName.c_str()), value);
}

void CProgram::SetUniform(std::string strName, float value)
{
    glUniform1f(glGetUniformLocation(m_IDProgram, strName.c_str()), value);
}

void CProgram::SetUniform(std::string strName, glm::vec2 value)
{
    glUniform2fv(glGetUniformLocation(m_IDProgram, strName.c_str()), 1, glm::value_ptr(value));
}

void CProgram::SetUniform(std::string strName, glm::vec3 value)
{
    glUniform3fv(glGetUniformLocation(m_IDProgram, strName.c_str()), 1, glm::value_ptr(value));
}

void CProgram::SetUniform(std::string strName, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_IDProgram, strName.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}