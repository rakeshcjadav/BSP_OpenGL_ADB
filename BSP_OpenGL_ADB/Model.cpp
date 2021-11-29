#include"pch.h"
#include"Model.h"

CModel* CModel::Load(std::string strFilePath)
{
    CModel* pModel = new CModel();
    if (!pModel->LoadModelPrivate(strFilePath))
    {
        delete pModel;
        pModel = nullptr;
    }
    return pModel;
}

CModel::CModel()
{

}

bool CModel::LoadModelPrivate(std::string strFilePath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(strFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene)
        return false;

    return true;
}