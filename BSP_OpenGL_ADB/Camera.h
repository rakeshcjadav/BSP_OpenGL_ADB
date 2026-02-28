#pragma once

class CViewport;
struct SCameraDef;
class CScene;

class CCamera : public IInputHandler
{
public:
    CCamera(SCameraDef* pCameraDef);
    void SetPosition(glm::vec3 pos);
    glm::vec3 GetPosition();
    void SetTarget(glm::vec3 target);
    void SetUp(glm::vec3 up);
    void Attach(CViewport* pViewport);
    glm::mat4 GetCameraMatrix();
    glm::mat4 GetPerspectiveProjectionMatrix();
    void SetScene(CScene* pScene);
    void Update();
    void Render();

    void OnKeyPressed(int key) override;
    void OnKeyReleased(int key) override;
    bool IsKeyPressed(int key) override;
    void OnMouseMove(double xpos, double ypos) override;
	void OnMouseScroll(double x, double y) override;
    void GetMousePos(double & xpos, double & ypos) override;
    void OnLeftMouseButtonPressed(int modifier) override;
    void OnRightMouseButtonPressed(int modifier) override;
    void OnLeftMouseButtonReleased(int modifier) override;
    void OnRightMouseButtonReleased(int modifier) override;

private:
	void PrintDetails();

private:
    CViewport* m_pViewport;
    SCameraDef* m_pCameraDef;
    glm::vec3 m_vPosition;
    glm::vec3 m_vDirection;
    glm::vec3 m_vUp;
    CScene* m_pScene;
    bool m_bDragging;
    bool m_bPanning;
    glm::dvec2 m_vMousePreviousPos;
    glm::vec3 m_vEulerAngles;
    float m_fCameraSensitivity;
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