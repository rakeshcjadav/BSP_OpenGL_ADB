#include"pch.h"
#include"Window.h"
#include"Viewport.h"

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
    int width = 1920;
    int height = 1080;
    CWindow* pWindow = nullptr;
    {
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        pWindow = new CWindow(width, height, "BSP OpenGL Course");

        // Initialize glew
        glewInit();

        pWindow->Prepare();

        glEnable(GL_MULTISAMPLE);

        // TODO: Move to material
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Transparent blend or Alpha blend or Transparent
        //glBlendFunc(GL_ONE, GL_ZERO); // Opaque or Replace blend
        //glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ZERO);

        // 1. Enable blending
        // glEnable(GL_BLEND);
        // 2.1 Define blend factors for whole color ( RGBA )
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // 2.2 Define separate blend factors for color (RGB) and alpha
        // glBlendFuncSeparate(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE, GL_ZERO);
        // 3.1 Define blend equation for whole color ( RGBA )
        // glBlendEquation(GL_FUNC_ADD);
        // 3.2 Define blend equation for color (RGB) blend and alpha blend
        // glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

    CViewport* pViewportLeft = new CViewport(glm::ivec4(0, 0, width, height));
    pWindow->Attach(pViewportLeft);

    //CViewport* pViewportRight = new CViewport(glm::ivec4(width/2, 0, width/2, height));
    //pWindow->Attach(pViewportRight);

    /* Loop until the user closes the window */
    while (!pWindow->IsWindowClosed())
    {
        /* Render here */
        pWindow->Render();

        //if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        //    glfwSetWindowShouldClose(window, true);

      //  float color[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
       // glUniform3fv(glGetUniformLocation(program, "uColor"), 2, color);
        
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