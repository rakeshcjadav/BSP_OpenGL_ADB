#pragma once

class CViewport;
class CCamera;
class CScene;

class CWindow : public IInputDelegator
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
    static void OnMouseButton(GLFWwindow* pWidnow, int buttonID, int action, int modifier);

    void HandleWindowResize(int width, int height) override;
    void HandleMouseMove(double xpos, double ypos) override;
    void GetMousePos(double & xpos, double & ypos) override;
    void HandleOnKey(int key, int scancode, int action, int modifier) override;
    void HandleMouseButton(int buttonID, int action, int modifier) override;

private:
    static std::map<GLFWwindow*, CWindow*> s_mapWindows;

    GLFWwindow* m_pGLFWWindow;
    //std::list<CViewport*> m_listViewports;
    CViewport* m_pViewport;
    CScene* m_pScene;
};