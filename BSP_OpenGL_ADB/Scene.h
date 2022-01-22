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
    CProgram* m_pUnlitProgram;
    CProgram* m_pLitProgram;
    CProgram* m_pLitDiffuseProgram;
    CProgram* m_pOutline;

    CTexture* m_pTexture;
    CTexture* m_pTexture2;
    CTexture* m_pContainerTexture;
    CTexture* m_pContainerSpecularTexture;
    CCamera* m_pCamera;

    CModel* m_pModelBackpack;

    CModel* m_pModelCrate;
    CModel* m_pModelMinion;
    CModel* m_pModelGround;

    CLight* m_pDirectionalLight;
    CLight* m_pSpotLight;
    CLight* m_pPointLight;

    CMaterial* m_pLitCrateMaterial;
    CMaterial* m_pUnlitCrateMaterial;
    CMaterial* m_pLitGround;

    CMaterial* m_pOutlineMaterial;
};