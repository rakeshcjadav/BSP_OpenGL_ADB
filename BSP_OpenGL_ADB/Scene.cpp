#include"pch.h"
#include"Scene.h"
#include"Program.h"
#include"Shader.h"
#include"Camera.h"
#include"Mesh.h"
#include"Model.h"
#include"DirectionalLight.h"
#include"Material.h"
#include"Transform.h"

CProgram* CreateProgram(std::string name, std::string vertShader, std::string fragShader)
{
    std::string strVertexShaderSource = CUtil::LoadShader(vertShader);
    std::string strFragmentShaderSource = CUtil::LoadShader(fragShader);

    CShader shaderVertex(CShader::SHADER_TYPE::VERTEX_SHADER, strVertexShaderSource);
    CShader shaderFragment(CShader::SHADER_TYPE::FRAGMENT_SHADER, strFragmentShaderSource);

    CProgram* pProgram = new CProgram(name, &shaderVertex, &shaderFragment);

    return pProgram;
}

CScene::CScene()
{

}

void CScene::LoadScene()
{
    m_pCamera = new CCamera(new SCameraDef(45.0f, 0.1f, 1000.0f));
    m_pCamera->SetPosition(glm::vec3(0.0f, 0.0f, 550.0f));
    m_pCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    m_pCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
    m_pCamera->SetScene(this);

    glm::vec3 sunLightDirection(1.0f, 1.0f, 1.0f);
    sunLightDirection = glm::normalize(sunLightDirection);
    glm::vec3 sunLightColor(1.0f, 1.0f, 1.0f);
    m_pDirectionalLight = new CDirectionalLight(sunLightDirection, sunLightColor);

    m_pModelBackpack = CModel::Load(
        new CTransform(glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(0.150f, 0.150f, 0.150f)),
        CUtil::GetModelPath() + "airplane\\11803_Airplane_v1_l1.obj");

}

CCamera* CScene::GetCamera()
{
    return m_pCamera;
}

void CScene::Render(CCamera* pCamera)
{   
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    m_pModelBackpack->Render(pCamera, nullptr, m_pDirectionalLight);
    glEnable(GL_DEPTH_TEST);
}