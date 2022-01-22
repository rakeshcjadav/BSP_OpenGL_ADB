#include"pch.h"
#include"Model.h"
#include"Mesh.h"
#include"Material.h"
#include"Camera.h"
#include"Texture.h"
#include"Shader.h"
#include"Program.h"
#include"Transform.h"

CProgram* CreateTempProgram(std::string name, std::string vertShader, std::string fragShader)
{
    std::string strVertexShaderSource = CUtil::LoadShader(vertShader);
    std::string strFragmentShaderSource = CUtil::LoadShader(fragShader);

    CShader shaderVertex(CShader::SHADER_TYPE::VERTEX_SHADER, strVertexShaderSource);
    CShader shaderFragment(CShader::SHADER_TYPE::FRAGMENT_SHADER, strFragmentShaderSource);

    CProgram* pProgram = new CProgram(name, &shaderVertex, &shaderFragment);

    return pProgram;
}

CModel* CModel::Load(CTransform * pTransform, std::string strFilePath)
{
    CModel* pModel = new CModel(pTransform);
    if (!pModel->LoadModelPrivate(strFilePath))
    {
        delete pModel;
        pModel = nullptr;
    }
    return pModel;
}

CModel* CModel::Load(CTransform* pTransform, CMesh* pMesh, CMaterial* pMaterial)
{
    CModel* pModel = new CModel(pTransform);
    if (!pModel->LoadModelPrivate(pMesh, pMaterial))
    {
        delete pModel;
        pModel = nullptr;
    }
    return pModel;
}

CModel::CModel(CTransform* pTransform):
    m_pTransform(pTransform)
{

}

void CModel::Render(CCamera* pCamera, CMaterial* pOverride, CLight * pDirectionalLight, CLight * pPointLight, CLight * pSpotLight)
{
    glm::mat4 matCamera = pCamera->GetCameraMatrix();
    glm::mat4 matProjection = pCamera->GetPerspectiveProjectionMatrix();
    glm::vec3 cameraPos = pCamera->GetPosition();
    for (auto pair : m_mapMeshes)
    {
        CMaterial* pMaterial = pOverride;
        if(!pMaterial)
            pMaterial = m_mapMaterials[pair.first];
        pMaterial->Bind();

        if(pDirectionalLight)
            pDirectionalLight->Bind(pMaterial);
        if(pPointLight)
            pPointLight->Bind(pMaterial);
        if(pSpotLight)
            pSpotLight->Bind(pMaterial);

        pMaterial->SetUniform("uMatCamera", matCamera);
        pMaterial->SetUniform("uMatProjection", matProjection);
        pMaterial->SetUniform("uMatModel", m_pTransform->GetMatrix());
        pMaterial->SetUniform("uCameraPos", cameraPos);

        for (CMesh* pMesh : pair.second)
        {
            pMesh->Render();
        }
    }
}

bool CModel::LoadModelPrivate(std::string strFilePath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(strFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    std::filesystem::path filePath = std::filesystem::path(strFilePath);
    filePath.remove_filename();
    std::string strTexturePath(filePath.string());

    if (!scene)
        return false;

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return false;
    }

    ProcessMaterials(scene, strTexturePath);

    ProcessNode(scene->mRootNode, scene);

    return true;
}

bool CModel::LoadModelPrivate(CMesh* pMesh, CMaterial* pMaterial)
{
    m_mapMaterials[pMaterial->GetName()] = pMaterial;
    m_mapMeshes[pMaterial->GetName()].push_back(pMesh);
    return true;
}

void CModel::ProcessMaterials(const aiScene* pScene, std::string strFilePath)
{
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
    {
        aiMaterial* material = pScene->mMaterials[i];
        
        aiString name;
        material->Get(AI_MATKEY_NAME, name);
        aiColor3D ambientColor;
        material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
        aiColor3D diffuseColor;
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        aiColor3D specularColor;
        material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        ai_real shininess;
        material->Get(AI_MATKEY_SHININESS, shininess);

        SMaterialDef* pMaterialDef = new SMaterialDef();
        pMaterialDef->ambientColor = glm::make_vec3(&ambientColor.r);
        pMaterialDef->diffuseColor = glm::make_vec3(&diffuseColor.r);
        pMaterialDef->specularColor = glm::make_vec3(&specularColor.r);
        pMaterialDef->shininess = shininess;
        pMaterialDef->specularStrength = specularColor.r;

        aiString diffuseTexture;
        material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), diffuseTexture);

        aiString specularTexture;
        material->Get(AI_MATKEY_TEXTURE_SPECULAR(0), specularTexture);

        std::map<std::string, CTexture*> mapTextures;
        if (diffuseTexture.length > 0)
        {
            mapTextures["uMaterial.DiffuseMap"] = new CTexture(strFilePath + diffuseTexture.C_Str());
        }
        if (specularTexture.length > 0)
        {
            mapTextures["uMaterial.SpecularMap"] = new CTexture(strFilePath + specularTexture.C_Str());
        }

        CProgram * pProgram = CreateTempProgram("lit_diffuse", "lit_diffuse.vert", "lit_diffuse.frag");

        m_mapMaterials[name.C_Str()] = new CMaterial(name.C_Str(), pMaterialDef, pProgram, mapTextures);
    }
}

void CModel::ProcessNode(aiNode* pNode, const aiScene* pScene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
    {
        aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
        std::string strMaterialName = pScene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str();
        m_mapMeshes[strMaterialName].push_back(ProcessMesh(mesh, pScene));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < pNode->mNumChildren; i++)
    {
        ProcessNode(pNode->mChildren[i], pScene);
    }
}

CMesh* CModel::ProcessMesh(aiMesh* pMesh, const aiScene* pScene)
{
    SMeshData meshData;
    
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
    {
        glm::vec3 position = glm::make_vec3(&pMesh->mVertices[i].x);
        glm::vec3 normal = glm::make_vec3(&pMesh->mNormals[i].x);
        glm::vec2 uv = glm::vec2(0.0f);
        if (pMesh->mTextureCoords[0])
        {
            uv = glm::make_vec2(&pMesh->mTextureCoords[0][i].x);
        }
        SVertex vertex(position, normal, uv);
        meshData.aVertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
    {
        aiFace face = pMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            meshData.aIndices.push_back(face.mIndices[j]);
    }

    return new CMesh(&meshData);
}