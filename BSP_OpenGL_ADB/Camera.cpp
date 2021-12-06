#include"pch.h"
#include"Camera.h"
#include"Viewport.h"
#include"Scene.h"

CCamera::CCamera(SCameraDef* pCameraDef)
{
    m_pCameraDef = pCameraDef;
    m_bDragging = false;
    m_vEulerAngles.y += -90.0f;
    m_fCameraSensitivity = 0.1f;
}

void CCamera::SetPosition(glm::vec3 pos)
{
    m_vPosition = pos;
}

glm::vec3 CCamera::GetPosition()
{
    return m_vPosition;
}

void CCamera::SetTarget(glm::vec3 target)
{
    m_vDirection = glm::normalize(target - m_vPosition);
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
    return glm::lookAt(m_vPosition, m_vPosition + m_vDirection, m_vUp);
}

glm::mat4 CCamera::GetPerspectiveProjectionMatrix()
{
    return glm::perspective(glm::radians(m_pCameraDef->fFOV), m_pViewport->GetAspectRatio(), m_pCameraDef->fNearPlane, m_pCameraDef->fFarPlane);
}

void CCamera::SetScene(CScene* pScene)
{
    m_pScene = pScene;
}

void CCamera::Update()
{
    if (IsKeyPressed(GLFW_KEY_W))
    {
        m_vPosition += m_vDirection * m_fCameraSensitivity;
    }
    else if (IsKeyPressed(GLFW_KEY_S))
    {
        m_vPosition -= m_vDirection * m_fCameraSensitivity;
    }
    else if (IsKeyPressed(GLFW_KEY_A))
    {
        glm::vec3 cameraRight = glm::normalize(glm::cross(m_vDirection, m_vUp)) * 1.0f;
        m_vPosition -= cameraRight * m_fCameraSensitivity;
    }
    else if (IsKeyPressed(GLFW_KEY_D))
    {
        glm::vec3 cameraRight = glm::normalize(glm::cross(m_vDirection, m_vUp)) * 1.0f;
        m_vPosition += cameraRight * m_fCameraSensitivity;
    }
}

void CCamera::Render()
{
    m_pScene->Render(this);
}

void CCamera::OnKeyPressed(int key)
{

}

void CCamera::OnKeyReleased(int key)
{

}

bool CCamera::IsKeyPressed(int key)
{
    if (m_pDelegator)
        return m_pDelegator->IsKeyPressed(key);
    return false;
}

void CCamera::OnMouseMove(double xpos, double ypos)
{
    //std::cout << "x: " << xpos << " y: " << ypos << std::endl;
    if (!m_bDragging)
        return;

    double xOffset = (xpos - m_vMousePreviousPos.x) * m_fCameraSensitivity;
    double yOffset = (m_vMousePreviousPos.y - ypos) * m_fCameraSensitivity;

    m_vEulerAngles.x += (float)yOffset;
    m_vEulerAngles.y += (float)xOffset;

    if (m_vEulerAngles.x > 89.0f)
        m_vEulerAngles.x = 89.0f;
    if (m_vEulerAngles.x < -89.0f)
        m_vEulerAngles.x = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_vEulerAngles.y)) * cos(glm::radians(m_vEulerAngles.x));
    direction.y = sin(glm::radians(m_vEulerAngles.x));
    direction.z = sin(glm::radians(m_vEulerAngles.y)) * cos(glm::radians(m_vEulerAngles.x));
    m_vDirection = glm::normalize(direction);

    glm::vec3 right = glm::normalize(glm::cross(m_vDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_vUp = glm::cross(right, m_vDirection);

    m_vMousePreviousPos.x = xpos;
    m_vMousePreviousPos.y = ypos;
}

void CCamera::GetMousePos(double& xpos, double& ypos)
{
    if(m_pDelegator)
        m_pDelegator->GetMousePos(xpos, ypos);
}

void CCamera::OnLeftMouseButtonPressed(int modifier)
{
    m_bDragging = true;
    //std::cout << "Left Mouse Button Pressed " << modifier << std::endl;
    GetMousePos(m_vMousePreviousPos.x, m_vMousePreviousPos.y);
    //std::cout << m_vMousePreviousPos.y << std::endl;
}

void CCamera::OnRightMouseButtonPressed(int modifier)
{

}

void CCamera::OnLeftMouseButtonReleased(int modifier)
{
    m_bDragging = false;
    //std::cout << "Left Mouse Button Released " << modifier << std::endl;
}

void CCamera::OnRightMouseButtonReleased(int modifier)
{

}