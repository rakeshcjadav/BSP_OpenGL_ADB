#pragma once

class IInputDelegator;

class IInputHandler
{
public:
    void AssignDelegator(IInputDelegator* pDelegator);

    virtual void OnKeyPressed(int key) = 0;
    virtual void OnKeyReleased(int key) = 0;
    virtual void OnMouseMove(double xpos, double ypos) = 0;
    virtual void GetMousePos(double& xpos, double& ypos) = 0;
    virtual void OnLeftMouseButtonPressed(int modifier) = 0;
    virtual void OnRightMouseButtonPressed(int modifier) = 0;
    virtual void OnLeftMouseButtonReleased(int modifier) = 0;
    virtual void OnRightMouseButtonReleased(int modifier) = 0;

protected:
    IInputDelegator* m_pDelegator;
};