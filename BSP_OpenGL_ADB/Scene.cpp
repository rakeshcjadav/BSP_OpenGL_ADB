#include"pch.h"
#include"Scene.h"
#include"Texture.h"
#include"Program.h"
#include"Shader.h"
#include"Camera.h"
#include"Mesh.h"
#include"Model.h"
#include"DirectionalLight.h"
#include"PointLight.h"
#include"SpotLight.h"
#include"Material.h"
#include"Transform.h"
#include"FrameBuffer.h"

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
    m_pCamera = new CCamera(new SCameraDef(45.0f, 0.1f, 100.0f));
    m_pCamera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    m_pCamera->SetTarget(glm::vec3(0.0f, 0.0f, -2.0f));
    m_pCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
    m_pCamera->SetScene(this);

    m_pUnlitProgram = CreateProgram("unlit_diffuse", "unlit_diffuse.vert", "unlit_diffuse.frag");
    m_pOutline = CreateProgram("outline", "outline.vert", "outline.frag");
    m_pLitProgram = CreateProgram("lit", "lit.vert", "lit.frag");
    m_pLitDiffuseProgram = CreateProgram("lit_diffuse", "lit_diffuse.vert", "lit_diffuse.frag");

    m_pTexture = new CFileTexture(CUtil::GetTexturePath() + "minions\\minion.png");
    m_pTexture2 = new CFileTexture(CUtil::GetTexturePath() + "minions\\minion.jpg");

    m_pContainerTexture = new CFileTexture(CUtil::GetTexturePath() + "container.png");
    m_pContainerSpecularTexture = new CFileTexture(CUtil::GetTexturePath() + "container_specular.png");

    glm::vec3 sunLightDirection(1.0f, 1.0f, 1.0f);
    sunLightDirection = glm::normalize(sunLightDirection);
    glm::vec3 sunLightColor(1.0f, 1.0f, 1.0f);
    m_pDirectionalLight = new CDirectionalLight(sunLightDirection, sunLightColor);

    glm::vec3 lightPos(5.0f, 2.0f, 0.0f);
    glm::vec3 lightColor(1.0f, 0.0f, 0.0f);
    glm::vec3 lightAttenuation(1.0f, 0.01f, 0.01f);
    m_pPointLight = new CPointLight(lightPos, lightColor, lightAttenuation);

    glm::vec3 spotLightPos(-2.0f, 2.0f, 4.0f);
    glm::vec3 spotLightDirection(0.0f, -1.0f, -1.0f);
    spotLightDirection = glm::normalize(spotLightDirection);
    glm::vec3 spotLightColor(0.0f, 0.0f, 1.0f);
    m_pSpotLight = new CSpotLight(
        spotLightPos,
        spotLightDirection,
        spotLightColor,
        lightAttenuation, glm::vec2(glm::radians(10.0f), glm::radians(60.0f)));

    {
        SMaterialDef* pMaterialDef = new SMaterialDef();
        pMaterialDef->ambientColor = glm::vec3(0.01f);
        pMaterialDef->diffuseColor = glm::vec3(1.0f);
        pMaterialDef->specularColor = glm::vec3(1.0f);
        pMaterialDef->shininess = 256.0f;
        pMaterialDef->specularStrength = 1.0f;

        std::map<std::string, CTexture*> mapTextures;
        mapTextures["uMaterial.DiffuseMap"] = m_pContainerTexture;
        mapTextures["uMaterial.SpecularMap"] = m_pContainerSpecularTexture;
        m_pLitGround = new CMaterial("Ground", pMaterialDef, m_pLitDiffuseProgram, mapTextures);
    }

    {
        SMaterialDef* pMaterialDef = new SMaterialDef();
        pMaterialDef->ambientColor = glm::vec3(0.01f);
        pMaterialDef->diffuseColor = glm::vec3(1.0f);
        pMaterialDef->specularColor = glm::vec3(1.0f);
        pMaterialDef->shininess = 256.0f;
        pMaterialDef->specularStrength = 1.0f;

        std::map<std::string, CTexture*> mapTextures;
        mapTextures["uMaterial.DiffuseMap"] = m_pContainerTexture;
        mapTextures["uMaterial.SpecularMap"] = m_pContainerSpecularTexture;
        m_pLitCrateMaterial = new CMaterial("Crate", pMaterialDef, m_pLitDiffuseProgram, mapTextures);
    }

    {
        SMaterialDef* pMaterialDef = new SMaterialDef();
        pMaterialDef->diffuseColor = glm::vec3(0.0f, 1.0f, 0.0f);
        m_pOutlineMaterial = new CMaterial("outline", pMaterialDef, m_pOutline);
    }

    {
        std::map<std::string, CTexture*> mapTextures;
        mapTextures["Texture"] = m_pTexture;
        mapTextures["Second"] = m_pTexture2;
        m_pUnlitCrateMaterial = new CMaterial("UnlitCrate", nullptr, m_pUnlitProgram, mapTextures);
    }

    m_pModelBackpack = CModel::Load(
        new CTransform(glm::vec3(0.0f, 2.0f, 0.0f)), 
        CUtil::GetModelPath() + "backpack\\backpack.obj");
    m_pModelCrate = CModel::Load(new CTransform(glm::vec3(2.0f, 0.0f, -4.0f)), CMesh::CreateCube(), m_pLitCrateMaterial);
    m_pModelMinion = CModel::Load(new CTransform(glm::vec3(-2.0f, 0.0f, -4.0f)), CMesh::CreateCube(), m_pUnlitCrateMaterial);
    m_pModelGround = CModel::Load(
        new CTransform(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(50.0f, 50.0f, 1.0f)),
        CMesh::CreateRectangle(), m_pLitGround);

    m_pModelFBO = CModel::Load(new CTransform(glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(0.0f), glm::vec3(5.0f, 5.0, 1.0)), CMesh::CreateCube(), m_pUnlitCrateMaterial);

    /*
    // World Space
    glm::mat4 matModel = glm::identity<glm::mat4>();
    matModel = glm::translate(matModel, glm::vec3(0.0f, 0.0f, -2.0f));
    matModel = glm::rotate(matModel, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //matModel = glm::scale(matModel, glm::vec3(500.0f/564.0f, 1.0f, 1.0f)); // minion.jpg
    matModel = glm::scale(matModel, glm::vec3(693.0f / 760.0f, 1.0f, 1.0f)); // minion.png

    //glm::mat4 matCamera = glm::identity<glm::mat4>();
    //matCamera = glm::translate(matCamera, glm::vec3(0.0f, 0.0f, -3.0f));
    //matCamera = glm::rotate(matCamera, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    m_pProgram->SetUniform("uMatModel", matModel);
    */

    m_pOffScreenFrameBuffer_MS = new CFrameBuffer("OffScreen_MultiSample", 1024, 1024, true);
    m_pOffScreenFrameBuffer = new CFrameBuffer("OffScreen", 1024, 1024, false);

    {
        std::map<std::string, CTexture*> mapTextures;
        mapTextures["Texture"] = m_pOffScreenFrameBuffer->GetColorAttachment();
        mapTextures["Second"] = m_pOffScreenFrameBuffer->GetNormalAttachment();
        m_pUnlitFBOMaterial = new CMaterial("UnlitFBO", nullptr, m_pUnlitProgram, mapTextures);
    }
}

CCamera* CScene::GetCamera()
{
    return m_pCamera;
}

void CScene::Render(CCamera* pCamera)
{
    // Render opaque objects
    // Depth Sorted : Objects far from camera will be rendered first
    // Render transparent objects

    // 1 Render Multisample FBO
    m_pOffScreenFrameBuffer_MS->Bind();
    m_pOffScreenFrameBuffer_MS->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    m_pModelBackpack->Render(pCamera, nullptr, m_pDirectionalLight, m_pPointLight, m_pSpotLight);
    m_pModelMinion->Render(pCamera);
    m_pOffScreenFrameBuffer_MS->UnBind();

    // 2 Copy From Multisample FBO to Singlesample FBO ( all color attachments ) 
    m_pOffScreenFrameBuffer_MS->ReadBind();
    m_pOffScreenFrameBuffer->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    m_pOffScreenFrameBuffer->DrawBind();
    m_pOffScreenFrameBuffer_MS->ReadBuffer(0);
    m_pOffScreenFrameBuffer->DrawBuffer(0);
    m_pOffScreenFrameBuffer->Blit();
    m_pOffScreenFrameBuffer_MS->ReadBuffer(1);
    m_pOffScreenFrameBuffer->DrawBuffer(1);
    m_pOffScreenFrameBuffer->Blit();
    m_pOffScreenFrameBuffer->UnBind();
    
    // 3 Final Render Pass on Default Framebuffer
    glStencilMask(0x00);

    m_pModelGround->Render(pCamera, nullptr, m_pDirectionalLight, m_pPointLight, m_pSpotLight);
    m_pModelFBO->Render(pCamera, m_pUnlitFBOMaterial, m_pDirectionalLight, m_pPointLight, m_pSpotLight);
    m_pModelMinion->Render(pCamera);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    m_pModelCrate->Render(pCamera, nullptr, m_pDirectionalLight, m_pPointLight, m_pSpotLight);
    m_pModelBackpack->Render(pCamera, nullptr, m_pDirectionalLight, m_pPointLight, m_pSpotLight);
    
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    m_pModelCrate->Render(pCamera, m_pOutlineMaterial, m_pDirectionalLight, m_pPointLight, m_pSpotLight);
    m_pModelBackpack->Render(pCamera, m_pOutlineMaterial, m_pDirectionalLight, m_pPointLight, m_pSpotLight);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

    
}