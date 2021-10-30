#include"pch.h"
#include"Window.h"
#include"Viewport.h"
#include"Camera.h"
#include"Mesh.h"
#include"Program.h"
#include"Shader.h"
#include"Texture.h"

std::string GetEnv(std::string strName)
{
    char* libvar;
    size_t requiredSize;
    getenv_s(&requiredSize, nullptr, 0, strName.c_str());
    if (requiredSize == 0)
    {
        //   LogMessage(fmt::format("Environment variable {0} doesn't exist!", varName).c_str());
        return nullptr;
    }

    libvar = (char*)malloc(requiredSize * sizeof(char));
    if (!libvar)
    {
        //   LogMessage(fmt::format("Failed to allocate memory {0} bytes!", requiredSize).c_str());
        return nullptr;
    }

    // Get the value of the LIB environment variable.
    getenv_s(&requiredSize, libvar, requiredSize, strName.c_str());

    return libvar;
}

std::string GetMediaPath()
{
    return GetEnv("MEDIA_PATH") + "\\";
}

std::string GetTexturePath()
{
    return GetMediaPath() + "textures\\";
}

CProgram* pProgram;
float Scale = 1.0f;

glm::mat4 matProjection = glm::identity<glm::mat4>();



CProgram * CreateShaders()
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

int main()
{
    /* GLM Introduction
    glm::vec4 point1(-0.5f, -0.5f, 0.0f, 1.0f); // A
    glm::vec4 point2(0.5f, -0.5f, 0.0f, 1.0f);  // B
    glm::vec4 point3(0.5f, 0.5f, 0.0f, 1.0f);   // C
    glm::vec4 point4(-0.5f, 0.5f, 0.0f, 1.0f);

    // BA
    glm::vec3 direction1 = glm::normalize(point2 - point1);

    // CA
    glm::vec3 direction2 = glm::normalize(point3 - point1);

    float consineAngle = glm::dot(direction1, direction2);
    float Angle = glm::acos(consineAngle) * 180.0f / glm::pi<float>();
    std::cout << glm::to_string(direction1) << std::endl;
    std::cout << glm::to_string(direction2) << std::endl;
    std::cout << Angle << std::endl;
    glm::vec3 normal = glm::normalize(glm::cross(direction1, direction2));
    std::cout << glm::to_string(normal) << std::endl;

    glm::mat4 matScale = glm::identity<glm::mat4>();
    matScale = glm::scale(matScale, glm::vec3(2.0f, 1.0f, 1.0f));
    std::cout << glm::to_string(matScale) << std::endl;

    glm::mat4 matRotate = glm::identity<glm::mat4>();
    matRotate = glm::rotate(matRotate, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    std::cout << glm::to_string(matRotate) << std::endl;

    glm::mat4 matTranslate = glm::identity<glm::mat4>();
    matTranslate = glm::translate(matTranslate, glm::vec3(1.0f, 0.0f, 0.0f));
    std::cout << glm::to_string(matTranslate) << std::endl;

    glm::mat4 matCombined = matTranslate * matRotate * matScale;
    std::cout << glm::to_string(matCombined) << std::endl;

    glm::vec4 scaledPoint1 = matCombined * point1;
    std::cout << glm::to_string(scaledPoint1) << std::endl;

    matCombined = glm::identity<glm::mat4>();
    matCombined = glm::translate(matCombined, glm::vec3(1.0f, 0.0f, 0.0f));
    matCombined = glm::rotate(matCombined, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    matCombined = glm::scale(matCombined, glm::vec3(2.0f, 1.0f, 1.0f));
    std::cout << glm::to_string(matCombined) << std::endl;

    scaledPoint1 = matCombined * point1;
    std::cout << glm::to_string(scaledPoint1) << std::endl;

    getchar();
    return 0;
    */
    int width = 1280;
    int height = 720;
    CWindow* pWindow = nullptr;
    {
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        pWindow = new CWindow(width, height, "BSP OpenGL Course");

        // Initialize glew
        glewInit();

        // TODO: Move to material
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Transparent blend or Alpha blend or Transparent
        //glBlendFunc(GL_ONE, GL_ZERO); // Opaque or Replace blend
        //glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ZERO);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_DEPTH_TEST);

    }

    CViewport* pViewport = new CViewport(glm::ivec4(0, 0, width, height));
    pWindow->Attach(pViewport);

    CCamera* pCamera = new CCamera(new SCameraDef(60.0f, 0.1f, 100.0f));
    pCamera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    pCamera->SetTarget(glm::vec3(0.0f, 0.0f, -2.0f));
    pCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
    pCamera->Attach(pViewport);

    pProgram = CreateShaders();

    pProgram->Use();
    pProgram->SetUniform("uScale", 1.0f);

    // World Space
    glm::mat4 matModel = glm::identity<glm::mat4>();
    matModel = glm::translate(matModel, glm::vec3(0.0f, 0.0f, -2.0f));
    matModel = glm::rotate(matModel, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //matModel = glm::scale(matModel, glm::vec3(500.0f/564.0f, 1.0f, 1.0f)); // minion.jpg
    matModel = glm::scale(matModel, glm::vec3(693.0f / 760.0f, 1.0f, 1.0f)); // minion.png

    //glm::mat4 matCamera = glm::identity<glm::mat4>();
    //matCamera = glm::translate(matCamera, glm::vec3(0.0f, 0.0f, -3.0f));
    //matCamera = glm::rotate(matCamera, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 matCamera = pCamera->GetCameraMatrix();

    matProjection = pCamera->GetPerspectiveProjectionMatrix();

    pProgram->SetUniform("uMatModel", matModel);
    pProgram->SetUniform("uMatCamera", matCamera);
    pProgram->SetUniform("uMatProjection", matProjection);

    // Model View Projection Matrix
    // World View Projection Matrix
    // World Camera Projection Matrix

    //unsigned int texture = LoadTexture(GetTexturePath() + "small_texture.png");
    //unsigned int texture = LoadTexture(GetTexturePath() + "minions\\minion.png");
    //unsigned int texture2 = LoadTexture(GetTexturePath() + "minions\\minion.jpg");
    CTexture* pTexture = new CTexture(GetTexturePath() + "minions\\minion.png");
    CTexture* pTexture2 = new CTexture(GetTexturePath() + "minions\\minion.jpg");

    CMesh* pMesh = CMesh::CreateRectangle();


    /* Loop until the user closes the window */
    while (!pWindow->IsWindowClosed())
    {
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        //    glfwSetWindowShouldClose(window, true);

        float time = (float)glfwGetTime();
        float fTime = 1.0f + fabs(sinf(time * 0.25f) * 2.0f);
        float fCos = cosf(time * 0.2f);
        pProgram->SetUniform("uTime", 1.0f);

      //  float color[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
       // glUniform3fv(glGetUniformLocation(program, "uColor"), 2, color);
        
        glm::mat4 matCamera = pCamera->GetCameraMatrix();

        {
            {
                glm::mat4 matModel = glm::identity<glm::mat4>();
                matModel = glm::translate(matModel, glm::vec3(0.0f, 0.0f, -4.0f));
                matModel = glm::rotate(matModel, glm::radians(fCos * 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                matModel = glm::scale(matModel, glm::vec3(500.0f / 564.0f, 1.0f, 1.0f)); // minion.jpg
                //matModel = glm::translate(matModel, glm::vec3(-0.5f, -0.5f, 0.0f));

                //matModel = glm::scale(matModel, glm::vec3(693.0f / 760.0f, 1.0f, 1.0f)); // minion.png
                pProgram->SetUniform("uMatModel", matModel);
                pProgram->SetUniform("uMatCamera", matCamera);
            }
            pProgram->Use();
            pTexture->Bind(0);
            pTexture2->Bind(1);
            /*
            glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
            glBindTexture(GL_TEXTURE_2D, texture);
            glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
            glBindTexture(GL_TEXTURE_2D, texture2);
            */
            pProgram->SetUniform("Texture", 1);
            pProgram->SetUniform("Second", 0);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //glDrawArrays(GL_TRIANGLES, 0, vertexCount);
            pMesh->Render();
        }

        {
            {
                glm::mat4 matModel = glm::identity<glm::mat4>();
                matModel = glm::translate(matModel, glm::vec3(0.4f, 0.2f, -3.0f));
                matModel = glm::rotate(matModel, glm::radians(fCos * 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                matModel = glm::scale(matModel, glm::vec3(500.0f / 564.0f, 1.0f, 1.0f)); // minion.jpg
                //matModel = glm::translate(matModel, glm::vec3(-0.5f, -0.5f, 0.0f));

                //matModel = glm::scale(matModel, glm::vec3(693.0f / 760.0f, 1.0f, 1.0f)); // minion.png
                pProgram->SetUniform("uMatModel", matModel);
                pProgram->SetUniform("uMatCamera", matCamera);
            }
            pProgram->Use();
            pTexture->Bind(0);
            pTexture2->Bind(1);
            //glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
            //glBindTexture(GL_TEXTURE_2D, texture);
            //glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
            //glBindTexture(GL_TEXTURE_2D, texture2);

            pProgram->SetUniform("Texture", 0);
            pProgram->SetUniform("Second", 1);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //glDrawArrays(GL_TRIANGLES, 0, vertexCount);
            pMesh->Render();
        }
        /*
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos.z += -0.2f;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos.z -= -0.2f;
        }
        */
        pWindow->SwapBuffers();
        pWindow->PollEvents();
    }

    glfwTerminate();
    return 0;
}