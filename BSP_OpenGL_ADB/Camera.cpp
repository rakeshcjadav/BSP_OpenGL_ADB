#include"pch.h"
#include"Camera.h"
#include"Viewport.h"

CCamera::CCamera(SCameraDef* pCameraDef)
{
    m_pCameraDef = pCameraDef;
}

void CCamera::SetPosition(glm::vec3 pos)
{
    m_vPosition = pos;
}

void CCamera::SetTarget(glm::vec3 target)
{
    m_vTarget = target;
}

void CCamera::SetUp(glm::vec3 up)
{
    m_vUp = up;
}

void CCamera::Attach(CViewport* pViewport)
{
    m_pViewport = pViewport;
}

glm::mat4 CCamera::GetCameraMatrix()
{
    return glm::lookAt(m_vPosition, m_vTarget, m_vUp);
}

glm::mat4 CCamera::GetPerspectiveProjectionMatrix()
{
    return glm::perspective(glm::radians(m_pCameraDef->fFOV), m_pViewport->GetAspectRatio(), m_pCameraDef->fNearPlane, m_pCameraDef->fFarPlane);
}