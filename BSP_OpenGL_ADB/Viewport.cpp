#include"pch.h"
#include"Viewport.h"

CViewport::CViewport(glm::ivec4 size)
{
    m_ViewportSize = size;
}

void CViewport::OnAttach()
{
    glViewport(m_ViewportSize.x, m_ViewportSize.y, m_ViewportSize.z, m_ViewportSize.w);
    glScissor(m_ViewportSize.x, m_ViewportSize.y, m_ViewportSize.z, m_ViewportSize.w);
}

float CViewport::GetAspectRatio()
{
    return m_ViewportSize.z / (m_ViewportSize.w * 1.0f);
}