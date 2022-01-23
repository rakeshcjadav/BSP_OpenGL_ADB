#pragma once

class CTexture
{
public:
    void Bind(unsigned int index);
    unsigned int GetID();
protected:
    CTexture(std::string strFileName);
    virtual ~CTexture();
protected:
    unsigned int m_IDTexture;
    std::string m_strName;
};


class CFileTexture : public CTexture
{
public:
    CFileTexture(std::string strFileName);
    virtual ~CFileTexture() override;
private:
    unsigned int LoadTexture(std::string strFileName);
};

class CDynamicTexture : public CTexture
{
public:
    CDynamicTexture(std::string strName, GLint internalformat, GLsizei width,  GLsizei height, GLint format, GLenum type);
    virtual ~CDynamicTexture() override;
};