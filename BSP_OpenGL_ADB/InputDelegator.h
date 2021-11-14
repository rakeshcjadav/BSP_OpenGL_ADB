#pragma once

class IInputDelegator
{
public:
    void AttachHandler(IInputHandler* pHandler);
    void DetachHandler(IInputHandler* pHandler);

    virtual void HandleWindowResize(int width, int height) = 0;
    virtual void HandleMouseMove(double xpos, double ypos) = 0;
    virtual void GetMousePos(double & xpos, double & ypos) = 0;
    virtual void HandleOnKey(int key, int scancode, int action, int modifier) = 0;
    virtual bool IsKeyPressed(int key) = 0;
    virtual void HandleMouseButton(int buttonID, int action, int modifier) = 0;

protected:
    std::list<IInputHandler*> m_listHandlers;
};