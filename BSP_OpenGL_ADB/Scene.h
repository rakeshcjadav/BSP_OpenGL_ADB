#pragma once

class CMesh;
class CProgram;
class CTexture;
class CCamera;

class CScene
{
public:
    CScene();
    void LoadScene();
    void Render(CCamera* pCamera);
    CCamera* GetCamera();

private:
    CMesh* m_pMesh;
    CMesh* m_pPlane;
    CProgram* m_pUnlitProgram;
    CProgram* m_pLitProgram;
    CTexture* m_pTexture;
    CTexture* m_pTexture2;
    CCamera* m_pCamera;
};