#pragma once

class CViewport
{
public:
    CViewport(glm::ivec4 size);
    void OnAttach();
    float GetAspectRatio();
private:
    glm::ivec4 m_ViewportSize;
};