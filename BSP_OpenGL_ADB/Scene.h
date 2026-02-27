#pragma once

class CModel;
class CMesh;
class CProgram;
class CCamera;

class CScene
{
public:
    CScene();
    void LoadScene();
    void Render(CCamera* pCamera);
    CCamera* GetCamera();

private:
    CCamera* m_pCamera;

    CModel* m_pModelBackpack;

    CLight* m_pDirectionalLight;
};