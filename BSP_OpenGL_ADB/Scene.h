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
    void ToggleWireframe();
    void SetWireframeThickness(float thickness);
    void AdjustWireframeThickness(float delta);
    void SetDisplacementScale(float scale);
    void AdjustDisplacementScale(float delta);
    void ToggleIsolines();
    void SetIsolineInterval(float interval);
    void AdjustIsolineInterval(float delta);
    void SetIsolineThickness(float thickness);
    void AdjustIsolineThickness(float delta);

private:
    CCamera* m_pCamera;

    CModel* m_pModelBackpack;
    CModel* m_pModelPlane;
	CMaterial* m_pMaterialPlane;

    CLight* m_pDirectionalLight;

    int m_iColormapMode;
    float m_fScalarRangeMin;
    float m_fScalarRangeMax;
    bool m_bShowWireframe;
    float m_fWireframeThickness;
    float m_fDisplacementScale;
    bool m_bShowIsolines;
    float m_fIsolineInterval;
    float m_fIsolineThickness;
};