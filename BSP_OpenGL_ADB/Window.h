#pragma once

class CViewport;

class CWindow
{
public:
    CWindow(int width, int height, std::string strName);

    bool IsWindowClosed();
    void SwapBuffers();
    void PollEvents();
    void Attach(CViewport* pViewport);
private:
    GLFWwindow* m_pGLFWWindow;
    std::list<CViewport*> m_listViewports;
};