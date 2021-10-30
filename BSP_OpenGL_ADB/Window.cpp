#include"pch.h"
#include"Window.h"
#include"Viewport.h"

void OnWindowResize(GLFWwindow* pWindow, int width, int height)
{
    //matProjection = glm::perspective(glm::radians(60.0f), width / (height * 1.0f), 0.1f, 100.0f);
    //pProgram->SetUniform("uMatProjection", matProjection);
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
}

void OnMouseScroll(GLFWwindow* pWindow, double x, double y)
{
    //Scale -= (float)y * 0.1f;
    //pProgram->SetUniform("uScale", Scale);
}

void OnKey(GLFWwindow* pWidnow, int key, int scancode, int action, int modifier)
{
    std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Modifier: " << modifier << std::endl;
}

CWindow::CWindow(int width, int height, std::string strName)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    m_pGLFWWindow = glfwCreateWindow(width, height, strName.c_str(), NULL, NULL);

    /* Make the window's context current */
    glfwMakeContextCurrent(m_pGLFWWindow);

    glfwSetFramebufferSizeCallback(m_pGLFWWindow, OnWindowResize);

    glfwSetScrollCallback(m_pGLFWWindow, OnMouseScroll);

    glfwSetKeyCallback(m_pGLFWWindow, OnKey);
}

bool CWindow::IsWindowClosed()
{
    return glfwWindowShouldClose(m_pGLFWWindow);
}

void CWindow::SwapBuffers()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_pGLFWWindow);
}

void CWindow::PollEvents()
{
    /* Poll for and process events */
    glfwPollEvents();
}

void CWindow::Attach(CViewport* pViewport)
{
    pViewport->OnAttach();
    m_listViewports.push_back(pViewport);
}