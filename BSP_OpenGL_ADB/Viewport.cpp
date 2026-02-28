#include"pch.h"
#include"Viewport.h"
#include"Camera.h"

CViewport::CViewport(glm::ivec4 size)
{
    m_ViewportSize = size;
}

void CViewport::Clear()
{
    glViewport(m_ViewportSize.x, m_ViewportSize.y, m_ViewportSize.z, m_ViewportSize.w);
    glScissor(m_ViewportSize.x, m_ViewportSize.y, m_ViewportSize.z, m_ViewportSize.w);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void CViewport::SetPosition(int x, int y)
{
    m_ViewportSize.x = x;
    m_ViewportSize.y = y;
}

void CViewport::SetSize(int width, int height)
{
    m_ViewportSize.z = width;
    m_ViewportSize.w = height;
}

float CViewport::GetAspectRatio()
{
    return m_ViewportSize.z / (m_ViewportSize.w * 1.0f);
}

void CViewport::Render(CCamera* pCamera)
{
    pCamera->Attach(this);
    pCamera->Update();
    pCamera->Render();
}