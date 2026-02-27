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
    /*
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
    */
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
    if (m_bDragging)
    {
        double xOffset = (xpos - m_vMousePreviousPos.x) * m_fCameraSensitivity;
        double yOffset = (ypos - m_vMousePreviousPos.y) * m_fCameraSensitivity;

        m_vEulerAngles.x += (float)yOffset;
        m_vEulerAngles.y += (float)xOffset;

        if (m_vEulerAngles.x > 89.0f)
            m_vEulerAngles.x = 89.0f;
        if (m_vEulerAngles.x < -89.0f)
            m_vEulerAngles.x = -89.0f;

        glm::vec3 worldOrigin(0.0f, 0.0f, 0.0f);
        float radius = glm::length(m_vPosition - worldOrigin);

        m_vPosition.x = worldOrigin.x + radius * cos(glm::radians(m_vEulerAngles.y)) * cos(glm::radians(m_vEulerAngles.x));
        m_vPosition.y = worldOrigin.y + radius * sin(glm::radians(m_vEulerAngles.x));
        m_vPosition.z = worldOrigin.z + radius * -sin(glm::radians(m_vEulerAngles.y)) * cos(glm::radians(m_vEulerAngles.x));

        m_vDirection = glm::normalize(worldOrigin - m_vPosition);

        glm::vec3 right = glm::normalize(glm::cross(m_vDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        m_vUp = glm::cross(right, m_vDirection);

        m_vMousePreviousPos.x = xpos;
        m_vMousePreviousPos.y = ypos;
    }
    else if (m_bPanning)
    {
        double xOffset = (xpos - m_vMousePreviousPos.x) * m_fCameraSensitivity;
        double yOffset = (ypos - m_vMousePreviousPos.y) * m_fCameraSensitivity;
        glm::vec3 right = glm::normalize(glm::cross(m_vDirection, m_vUp));
        glm::vec3 up = glm::normalize(glm::cross(right, m_vDirection));
        m_vPosition -= right * (float)xOffset;
        m_vPosition += up * (float)yOffset;
        m_vMousePreviousPos.x = xpos;
        m_vMousePreviousPos.y = ypos;
	}
}

void CCamera::PrintDetails()
{
    std::cout << "Camera Position: (" << m_vPosition.x << ", " << m_vPosition.y << ", " << m_vPosition.z << ")" << std::endl;
    std::cout << "Camera Direction: (" << m_vDirection.x << ", " << m_vDirection.y << ", " << m_vDirection.z << ")" << std::endl;
    std::cout << "Camera Up: (" << m_vUp.x << ", " << m_vUp.y << ", " << m_vUp.z << ")" << std::endl;
	std::cout << "Camera FOV: " << m_pCameraDef->fFOV << std::endl;
}

void CCamera::OnMouseScroll(double x, double y)
{
	// Zoom in/out
    m_pCameraDef->fFOV -= (float)y;
    if (m_pCameraDef->fFOV < 1.0f)
        m_pCameraDef->fFOV = 1.0f;
    if (m_pCameraDef->fFOV > 90.0f)
		m_pCameraDef->fFOV = 90.0f;
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
	m_bPanning = true;
    GetMousePos(m_vMousePreviousPos.x, m_vMousePreviousPos.y);
}

void CCamera::OnLeftMouseButtonReleased(int modifier)
{
    m_bDragging = false;
    //std::cout << "Left Mouse Button Released " << modifier << std::endl;
}

void CCamera::OnRightMouseButtonReleased(int modifier)
{
    m_bPanning = false;
}