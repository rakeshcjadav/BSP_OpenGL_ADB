#pragma once

class CViewport;
class CCamera;
class CScene;

class CWindow
{
public:
    CWindow(int width, int height, std::string strName);

    void Prepare();
    bool IsWindowClosed();
    void SwapBuffers();
    void PollEvents();
    void Attach(CViewport* pViewport);
    void Render();

    static void OnWindowResize(GLFWwindow* pWindow, int width, int height);
    static void OnMouseMove(GLFWwindow* pWidnow, double xpos, double ypos);
    static void OnKey(GLFWwindow* pWidnow, int key, int scancode, int action, int modifier);


    void HandleWindowResize(int width, int height);
    void HandleMouseMove(double xpos, double ypos);
    void HandleOnKey(int key, int scancode, int action, int modifier);

private:
    static std::map<GLFWwindow*, CWindow*> s_mapWindows;

    GLFWwindow* m_pGLFWWindow;
    //std::list<CViewport*> m_listViewports;
    CViewport* m_pViewport;
    CScene* m_pScene;
};