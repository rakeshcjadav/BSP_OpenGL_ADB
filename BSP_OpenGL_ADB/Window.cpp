#include"pch.h"
#include"Window.h"
#include"Viewport.h"
#include"Scene.h"
#include"Camera.h"

std::map<GLFWwindow*, CWindow*> CWindow::s_mapWindows;

void CWindow::OnWindowResize(GLFWwindow* pGLFWWindow, int width, int height)
{
    CWindow * pWindow = s_mapWindows[pGLFWWindow];
    pWindow->HandleWindowResize(width, height);
}

void OnMouseScroll(GLFWwindow* pWindow, double x, double y)
{
    //Scale -= (float)y * 0.1f;
    //pProgram->SetUniform("uScale", Scale);
}

void CWindow::OnMouseMove(GLFWwindow* pGLFWWindow, double xpos, double ypos)
{
    CWindow* pWindow = s_mapWindows[pGLFWWindow];
    pWindow->HandleMouseMove(xpos, ypos);
}

void CWindow::OnKey(GLFWwindow* pGLFWWindow, int key, int scancode, int action, int modifier)
{
    CWindow* pWindow = s_mapWindows[pGLFWWindow];
    pWindow->HandleOnKey(key, scancode, action, modifier);
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

    glfwSetFramebufferSizeCallback(m_pGLFWWindow, CWindow::OnWindowResize);
    glfwSetScrollCallback(m_pGLFWWindow, OnMouseScroll);
    glfwSetCursorPosCallback(m_pGLFWWindow, CWindow::OnMouseMove);
    glfwSetKeyCallback(m_pGLFWWindow, CWindow::OnKey);

    s_mapWindows[m_pGLFWWindow] = this;
}

void CWindow::Prepare()
{
    m_pScene = new CScene();
    m_pScene->LoadScene();
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
    //m_listViewports.push_back(pViewport);
    m_pViewport = pViewport;
}

void CWindow::Render()
{
    /*
    for (CViewport* pViewport : m_listViewports)
    {
        pViewport->Clear();
        pViewport->Render(m_pScene->GetCamera());
    }*/
    m_pViewport->Clear();
    m_pViewport->Render(m_pScene->GetCamera());
}

void CWindow::HandleWindowResize(int width, int height)
{
    m_pViewport->SetSize(width, height);
}

void CWindow::HandleMouseMove(double xpos, double ypos)
{
    CCamera* pCamera = m_pScene->GetCamera();
    pCamera->OnMouseMove(xpos, ypos);
}

void CWindow::HandleOnKey(int key, int scancode, int action, int modifier)
{
    CCamera * pCamera = m_pScene->GetCamera();
    std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Modifier: " << modifier << std::endl;
    if (action == GLFW_PRESS)
    {
        pCamera->OnKeyPressed(key);
    }
    else if (action == GLFW_RELEASE)
    {
        pCamera->OnKeyReleased(key);
    }
}