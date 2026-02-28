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
    m_fDisplacementScale = 0.0f;
    m_bShowIsolines = false;
    m_fIsolineInterval = 0.15f;
    m_fIsolineThickness = 1.0f;
    m_bUseComputeNormals = false;
    m_pComputeProgram = nullptr;
    m_bSmoothNormals = false;
    m_pSmoothNormalsProgram = nullptr;
    m_bShowPlane = true;
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

	m_pMaterialPlane = new CMaterial("plane_material", new SMaterialDef(
                
        ), CreateProgram("plane_program", "lit.vert", "lit.frag"));
    m_pModelPlane = CModel::Load(
        new CTransform(glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(300.0f, 300.0f, 300.0f)),
		CMesh::CreateRectangleTriangleList(), m_pMaterialPlane);

	std::string strComputeShaderSource = CUtil::LoadShader("recompute_normals.comp");
    CShader computeShader(CShader::SHADER_TYPE::COMPUTE_SHADER, strComputeShaderSource);
    m_pComputeProgram = new CProgram("compute_normals", &computeShader);

    std::string strSmoothShaderSource = CUtil::LoadShader("smooth_scalars.comp");
    CShader smoothShader(CShader::SHADER_TYPE::COMPUTE_SHADER, strSmoothShaderSource);
    m_pSmoothProgram = new CProgram("smooth_scalars", &smoothShader);

    std::string strCopyShaderSource = CUtil::LoadShader("copy_scalars.comp");
    CShader copyShader(CShader::SHADER_TYPE::COMPUTE_SHADER, strCopyShaderSource);
    m_pCopyProgram = new CProgram("copy_scalars", &copyShader);

    std::string strSmoothNormalsSource = CUtil::LoadShader("smooth_normals.comp");
    CShader smoothNormalsShader(CShader::SHADER_TYPE::COMPUTE_SHADER, strSmoothNormalsSource);
    m_pSmoothNormalsProgram = new CProgram("smooth_normals", &smoothNormalsShader);

    std::string strCopyNormalsSource = CUtil::LoadShader("copy_normals.comp");
    CShader copyNormalsShader(CShader::SHADER_TYPE::COMPUTE_SHADER, strCopyNormalsSource);
    m_pCopyNormalsProgram = new CProgram("copy_normals", &copyNormalsShader);
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

void CScene::SetDisplacementScale(float scale)
{
    m_fDisplacementScale = glm::clamp(scale, 0.0f, 10.0f);
    std::cout << "Displacement Scale: " << m_fDisplacementScale << std::endl;
}

void CScene::AdjustDisplacementScale(float delta)
{
    m_fDisplacementScale = glm::clamp(m_fDisplacementScale + delta, 0.0f, 10.0f);
    std::cout << "Displacement Scale: " << m_fDisplacementScale << std::endl;
}

void CScene::ToggleIsolines()
{
    m_bShowIsolines = !m_bShowIsolines;
    std::cout << "Isolines: " << (m_bShowIsolines ? "ON" : "OFF") << std::endl;
}

void CScene::SetIsolineInterval(float interval)
{
    m_fIsolineInterval = glm::clamp(interval, 0.05f, 2.0f);
    std::cout << "Isoline Interval: " << m_fIsolineInterval << std::endl;
}

void CScene::AdjustIsolineInterval(float delta)
{
    m_fIsolineInterval = glm::clamp(m_fIsolineInterval + delta, 0.05f, 2.0f);
    std::cout << "Isoline Interval: " << m_fIsolineInterval << std::endl;
}

void CScene::SetIsolineThickness(float thickness)
{
    m_fIsolineThickness = glm::clamp(thickness, 0.5f, 5.0f);
    std::cout << "Isoline Thickness: " << m_fIsolineThickness << std::endl;
}

void CScene::AdjustIsolineThickness(float delta)
{
    m_fIsolineThickness = glm::clamp(m_fIsolineThickness + delta, 0.5f, 5.0f);
    std::cout << "Isoline Thickness: " << m_fIsolineThickness << std::endl;
}


void CScene::ToggleComputeNormals()
{
    m_bUseComputeNormals = !m_bUseComputeNormals;
    std::cout << "Compute Normals: " << (m_bUseComputeNormals ? "ON" : "OFF") << std::endl;
}

void CScene::ToggleSmoothNormals()
{
    m_bSmoothNormals = !m_bSmoothNormals;
    std::cout << "Smooth Normals: " << (m_bSmoothNormals ? "ON" : "OFF") << std::endl;
}

void CScene::ToggleModel()
{
    m_bShowPlane = !m_bShowPlane;
    std::cout << "Showing: " << (m_bShowPlane ? "PLANE (Visualization Mesh)" : "AIRPLANE (3D Model)") << std::endl;
}

void CScene::Render(CCamera* pCamera)
{   
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    
    if (m_bShowPlane)
    {
        if (m_bUseComputeNormals && m_pComputeProgram && m_fDisplacementScale > 0.0f)
        {
            m_pModelPlane->RunComputeNormalsShader(m_pComputeProgram, m_fDisplacementScale);
        }

        if (m_bSmoothNormals)
        {
            m_pModelPlane->RunSmoothNormalsShader(m_pSmoothNormalsProgram, m_pCopyNormalsProgram);
        }
        
        m_pModelPlane->Render(pCamera, nullptr, m_pDirectionalLight, m_iColormapMode, m_fScalarRangeMin, m_fScalarRangeMax, m_bShowWireframe, m_fWireframeThickness, m_fDisplacementScale, m_bShowIsolines, m_fIsolineInterval, m_fIsolineThickness);
    }
    else
    {
        m_pModelBackpack->Render(pCamera, nullptr, m_pDirectionalLight, m_iColormapMode, m_fScalarRangeMin, m_fScalarRangeMax, m_bShowWireframe, m_fWireframeThickness, m_fDisplacementScale, m_bShowIsolines, m_fIsolineInterval, m_fIsolineThickness);
    }
    
    glEnable(GL_DEPTH_TEST);
}
