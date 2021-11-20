#include"pch.h"
#include"Scene.h"
#include"Texture.h"
#include"Program.h"
#include"Shader.h"
#include"Camera.h"
#include"Mesh.h"

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
    m_pUnlitProgram = CreateProgram("unlit_diffuse", "unlit_diffuse.vert", "unlit_diffuse.frag");
    m_pLitProgram = CreateProgram("lit", "lit.vert", "lit.frag");

    m_pTexture = new CTexture(CUtil::GetTexturePath() + "minions\\minion.png");
    m_pTexture2 = new CTexture(CUtil::GetTexturePath() + "minions\\minion.jpg");

    m_pMesh = CMesh::CreateCube();

    m_pCamera = new CCamera(new SCameraDef(60.0f, 0.1f, 100.0f));
    m_pCamera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    m_pCamera->SetTarget(glm::vec3(0.0f, 0.0f, -2.0f));
    m_pCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
    m_pCamera->SetScene(this);

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
}

CCamera* CScene::GetCamera()
{
    return m_pCamera;
}

void CScene::Render(CCamera* pCamera)
{
    glm::mat4 matCamera = pCamera->GetCameraMatrix();
    glm::mat4 matProjection = pCamera->GetPerspectiveProjectionMatrix();
    glm::vec3 cameraPos = pCamera->GetPosition();
    {
        m_pUnlitProgram->Use();
        m_pUnlitProgram->SetUniform("uMatCamera", matCamera);
        m_pUnlitProgram->SetUniform("uMatProjection", matProjection);
        glm::mat4 matModel = glm::identity<glm::mat4>();
        matModel = glm::translate(matModel, glm::vec3(2.0f, 0.0f, -4.0f));
        matModel = glm::rotate(matModel, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        matModel = glm::scale(matModel, glm::vec3(500.0f / 564.0f, 1.0f, 1.0f)); // minion.jpg
        //matModel = glm::translate(matModel, glm::vec3(-0.5f, -0.5f, 0.0f));

        //matModel = glm::scale(matModel, glm::vec3(693.0f / 760.0f, 1.0f, 1.0f)); // minion.png
        m_pUnlitProgram->SetUniform("uMatModel", matModel);
        m_pTexture->Bind(0);
        m_pTexture2->Bind(1);
        m_pUnlitProgram->SetUniform("Texture", 1);
        m_pUnlitProgram->SetUniform("Second", 0);
    }
    m_pMesh->Render();

    glm::vec3 lightPos(0.0f, 2.0f, -2.0f);
    {
        m_pLitProgram->Use();
        m_pLitProgram->SetUniform("uMatCamera", matCamera);
        m_pLitProgram->SetUniform("uMatProjection", matProjection);
        glm::mat4 matModel = glm::identity<glm::mat4>();
        matModel = glm::translate(matModel, glm::vec3(-2.0f, 0.0f, -4.0f));
        matModel = glm::rotate(matModel, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        matModel = glm::scale(matModel, glm::vec3(500.0f / 564.0f, 1.0f, 1.0f)); // minion.jpg
        //matModel = glm::translate(matModel, glm::vec3(-0.5f, -0.5f, 0.0f));

        //matModel = glm::scale(matModel, glm::vec3(693.0f / 760.0f, 1.0f, 1.0f)); // minion.png
        m_pLitProgram->SetUniform("uMatModel", matModel);
    }
    {
        glm::mat4 matModel = glm::identity<glm::mat4>();
        matModel = glm::rotate(matModel, glm::radians((float)glfwGetTime()*20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        matModel = glm::translate(matModel, glm::vec3(-4.0f, 0.0f, -2.0f));
       // lightPos = matModel * glm::vec4(lightPos, 1.0f);
        std::cout << glm::to_string(lightPos) << std::endl;
        m_pLitProgram->SetUniform("uLightPos", lightPos);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        m_pLitProgram->SetUniform("uLightColor", lightColor);
    }
    {
        m_pLitProgram->SetUniform("uCameraPos", cameraPos);
    }
    {
        m_pLitProgram->SetUniform("uObjectColor", glm::vec3(0.5f, 0.3f, 0.1f));
    }
    m_pMesh->Render();
}