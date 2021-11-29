#pragma once

class CMesh;

class CModel
{
public:
    static CModel* Load(std::string strFilePath);

private:
    CModel();
    bool LoadModelPrivate(std::string strFilePath);
private:
    std::list<CMesh*> m_listMeshes;

};