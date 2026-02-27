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
    void SetColormapMode(int mode);
    void SetScalarRangeMin(float value);
    void SetScalarRangeMax(float value);
    void AdjustScalarRangeMin(float delta);
    void AdjustScalarRangeMax(float delta);

private:
    CCamera* m_pCamera;

    CModel* m_pModelBackpack;

    CLight* m_pDirectionalLight;

    int m_iColormapMode;
    float m_fScalarRangeMin;
    float m_fScalarRangeMax;
};