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
    m_iColormapMode = 0;
    m_fScalarRangeMin = 0.0f;
    m_fScalarRangeMax = 1.0f;
    m_bShowWireframe = false;
    m_fWireframeThickness = 1.0f;
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

	m_pMaterialPlane = new CMaterial("plane_material", nullptr, CreateProgram("plane_program", "lit.vert", "lit.frag"));
    m_pModelPlane = CModel::Load(
        new CTransform(glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(300.0f, 300.0f, 300.0f)),
		CMesh::CreateRectangleTriangleList(), m_pMaterialPlane);

}

CCamera* CScene::GetCamera()
{
    return m_pCamera;
}

void CScene::SetColormapMode(int mode)
{
    m_iColormapMode = mode;
    std::string colormap = (mode == 0) ? "Viridis" : (mode == 1) ? "Turbo" : "Jet";
    std::cout << "Colormap changed to: " << colormap << std::endl;
}

void CScene::SetScalarRangeMin(float value)
{
    m_fScalarRangeMin = glm::clamp(value, 0.0f, m_fScalarRangeMax);
    std::cout << "Scalar Range: [" << m_fScalarRangeMin << ", " << m_fScalarRangeMax << "]" << std::endl;
}

void CScene::SetScalarRangeMax(float value)
{
    m_fScalarRangeMax = glm::clamp(value, m_fScalarRangeMin, 1.0f);
    std::cout << "Scalar Range: [" << m_fScalarRangeMin << ", " << m_fScalarRangeMax << "]" << std::endl;
}

void CScene::AdjustScalarRangeMin(float delta)
{
    m_fScalarRangeMin = glm::clamp(m_fScalarRangeMin + delta, 0.0f, m_fScalarRangeMax);
    std::cout << "Scalar Range: [" << m_fScalarRangeMin << ", " << m_fScalarRangeMax << "]" << std::endl;
}

void CScene::AdjustScalarRangeMax(float delta)
{
    m_fScalarRangeMax = glm::clamp(m_fScalarRangeMax + delta, m_fScalarRangeMin, 1.0f);
    std::cout << "Scalar Range: [" << m_fScalarRangeMin << ", " << m_fScalarRangeMax << "]" << std::endl;
}

void CScene::ToggleWireframe()
{
    m_bShowWireframe = !m_bShowWireframe;
    std::cout << "Wireframe: " << (m_bShowWireframe ? "ON" : "OFF") << std::endl;
}

void CScene::SetWireframeThickness(float thickness)
{
    m_fWireframeThickness = glm::clamp(thickness, 0.1f, 5.0f);
    std::cout << "Wireframe Thickness: " << m_fWireframeThickness << std::endl;
}

void CScene::AdjustWireframeThickness(float delta)
{
    m_fWireframeThickness = glm::clamp(m_fWireframeThickness + delta, 0.1f, 5.0f);
    std::cout << "Wireframe Thickness: " << m_fWireframeThickness << std::endl;
}

void CScene::Render(CCamera* pCamera)
{   
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    m_pModelPlane->Render(pCamera, nullptr, m_pDirectionalLight, m_iColormapMode, m_fScalarRangeMin, m_fScalarRangeMax, m_bShowWireframe, m_fWireframeThickness);
    glEnable(GL_DEPTH_TEST);
}