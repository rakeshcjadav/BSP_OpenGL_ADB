#pragma once

class CModel;
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
    CProgram* m_pLitDiffuseProgram;
    CTexture* m_pTexture;
    CTexture* m_pTexture2;
    CTexture* m_pContainerTexture;
    CTexture* m_pContainerSpecularTexture;
    CCamera* m_pCamera;

    CModel* m_pModelBackpack;
};