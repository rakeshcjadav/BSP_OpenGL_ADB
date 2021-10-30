#pragma once

class CViewport;
struct SCameraDef;

class CCamera
{
public:
    CCamera(SCameraDef* pCameraDef);
    void SetPosition(glm::vec3 pos);
    void SetTarget(glm::vec3 target);
    void SetUp(glm::vec3 up);
    void Attach(CViewport* pViewport);
    glm::mat4 GetCameraMatrix();
    glm::mat4 GetPerspectiveProjectionMatrix();

private:
    CViewport * m_pViewport;
    SCameraDef* m_pCameraDef;
    glm::vec3 m_vPosition;
    glm::vec3 m_vTarget;
    glm::vec3 m_vUp;
};


struct SCameraDef
{
    float fFOV;
    float fNearPlane;
    float fFarPlane;

    SCameraDef(float _fov, float _fNear, float _fFar)
    {
        fFOV = _fov;
        fNearPlane = _fNear;
        fFarPlane = _fFar;
    }
};