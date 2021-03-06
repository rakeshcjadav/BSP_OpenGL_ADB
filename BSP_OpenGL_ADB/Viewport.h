#pragma once

class CCamera;

class CViewport
{
public:
    CViewport(glm::ivec4 size);
    void Clear();
    float GetAspectRatio();
    void SetPosition(int x, int y);
    void SetSize(int width, int height);
    void Render(CCamera* pCamera);
private:
    glm::ivec4 m_ViewportSize;
};