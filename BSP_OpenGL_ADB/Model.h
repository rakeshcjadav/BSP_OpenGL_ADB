#pragma once

class CMesh;
class CCamera;
class CMaterial;
class CTransform;

class CModel
{
public:
    static CModel* Load(CTransform* pTransform, std::string strFilePath);
    static CModel* Load(CTransform* pTransform, CMesh * pMesh, CMaterial * pMaterial);
    void Render(CCamera* pCamera, CMaterial* pOverride = nullptr, CLight* pDirectionalLight = nullptr, int colormapMode = 0, float scalarMin = 0.0f, float scalarMax = 1.0f, bool showWireframe = false, float wireframeThickness = 1.0f);

private:
    CModel(CTransform* pTransform);
    bool LoadModelPrivate(std::string strFilePath);
    bool LoadModelPrivate(CMesh* pMesh, CMaterial* pMaterial);
    void ProcessMaterials(const aiScene* pScene, std::string strFilePath);
    void ProcessNode(aiNode* pNode, const aiScene* pScene);
    CMesh* ProcessMesh(aiMesh* pMesh, const aiScene* pScene);
private:
    //std::list<CMesh*> m_listMeshes;
    typedef std::list<CMesh*> MESHES;
    std::map<std::string, MESHES> m_mapMeshes;

    std::map<std::string, CMaterial*> m_mapMaterials;
    CTransform* m_pTransform;
};