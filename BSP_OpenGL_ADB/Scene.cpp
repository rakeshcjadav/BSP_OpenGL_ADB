#include"pch.h"
#include"Scene.h"
#include"Texture.h"
#include"Program.h"
#include"Shader.h"
#include"Camera.h"
#include"Mesh.h"

CProgram* CreateShaders()
{
    const char* VertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec4 aPos;\n"
        "layout(location = 1) in vec2 TexCoord;\n"
        "//layout(location = 1) in vec3 Color;\n"
        "uniform vec3 uColor;\n"
        "uniform float uTime;\n"
        "uniform mat4 uMatModel;\n"
        "uniform mat4 uMatCamera;\n"
        "uniform mat4 uMatProjection;\n"
        "out vec3 colorFrag;\n"
        "out vec2 UVFrag;\n"
        "const float PI = 3.14;\n"
        "const float Angle = 90.0f * PI/180.0f;"
        "void main()\n"
        "{\n"
        "gl_Position = uMatProjection * uMatCamera * uMatModel * aPos;\n"
        "UVFrag = TexCoord;\n"
        "}\n";

    const char* FragmentShaderSource =
        "#version 330 core\n"
        "in vec3 colorFrag;\n"
        "in vec2 UVFrag;\n"
        "uniform vec3 uColor[2];\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D Texture;\n"
        "uniform sampler2D Second;\n"
        "void main()\n"
        "{\n"
        "vec2 scaledUV = (UVFrag - vec2(0.5)) * vec2(2.0) + vec2(0.5);\n"
        "vec2 scaledUV1 = UVFrag * vec2(2.0) - vec2(0.5);\n"
        "vec4 colorMinionOne = texture(Texture, UVFrag);\n"
        "//vec4 colorMinionTwo = texture(Second, UVFrag);\n"
        "//vec4 colorMultiply = colorMinionOne* colorMinionTwo;\n"
        "//vec4 finalColor = colorMinionOne*colorMinionOne.a + colorMinionTwo*(1.0 - colorMinionOne.a);\n"
        "FragColor = colorMinionOne;\n"
        "}\n";

    CShader shaderVertex(CShader::SHADER_TYPE::VERTEX_SHADER, VertexShaderSource);
    CShader shaderFragment(CShader::SHADER_TYPE::FRAGMENT_SHADER, FragmentShaderSource);

    CProgram* pProgram = new CProgram("default", &shaderVertex, &shaderFragment);

    return pProgram;
}

CScene::CScene()
{

}

void CScene::LoadScene()
{
    m_pProgram = CreateShaders();

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
    m_pProgram->Use();
    m_pProgram->SetUniform("uMatCamera", matCamera);
    m_pProgram->SetUniform("uMatProjection", matProjection);
    {
        glm::mat4 matModel = glm::identity<glm::mat4>();
        matModel = glm::translate(matModel, glm::vec3(-0.4f, 0.0f, -4.0f));
        matModel = glm::rotate(matModel, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        matModel = glm::scale(matModel, glm::vec3(500.0f / 564.0f, 1.0f, 1.0f)); // minion.jpg
        //matModel = glm::translate(matModel, glm::vec3(-0.5f, -0.5f, 0.0f));

        //matModel = glm::scale(matModel, glm::vec3(693.0f / 760.0f, 1.0f, 1.0f)); // minion.png
        m_pProgram->SetUniform("uMatModel", matModel);
    }
    m_pTexture->Bind(0);
    m_pTexture2->Bind(1);
    m_pProgram->SetUniform("Texture", 1);
    m_pProgram->SetUniform("Second", 0);
    m_pMesh->Render();
    /*
    {
        glm::mat4 matModel = glm::identity<glm::mat4>();
        matModel = glm::translate(matModel, glm::vec3(0.4f, 0.2f, -3.0f));
        matModel = glm::rotate(matModel, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        matModel = glm::scale(matModel, glm::vec3(500.0f / 564.0f, 1.0f, 1.0f)); // minion.jpg
        //matModel = glm::translate(matModel, glm::vec3(-0.5f, -0.5f, 0.0f));

        //matModel = glm::scale(matModel, glm::vec3(693.0f / 760.0f, 1.0f, 1.0f)); // minion.png
        m_pProgram->SetUniform("uMatModel", matModel);
    }
    m_pProgram->SetUniform("Texture", 1);
    m_pProgram->SetUniform("Second", 0);
    m_pMesh->Render();*/
}