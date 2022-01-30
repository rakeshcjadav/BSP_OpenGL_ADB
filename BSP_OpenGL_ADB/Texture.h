#pragma once

class CTexture
{
public:
    void Bind(unsigned int index);
    void UnBind();
    unsigned int GetID();
    virtual GLenum GetType() = 0;
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
    virtual GLenum GetType() override;
private:
    unsigned int LoadTexture(std::string strFileName);
};

class CDynamicTexture : public CTexture
{
public:
    CDynamicTexture(std::string strName, GLint internalformat, GLsizei width,  GLsizei height, GLint format, GLenum type);
    virtual ~CDynamicTexture() override;
    virtual GLenum GetType() override;
};

class CDynamicMultiSampleTexture : public CTexture
{
public:
    CDynamicMultiSampleTexture(std::string strName, GLint internalformat, GLsizei width, GLsizei height, GLsizei samples);
    virtual ~CDynamicMultiSampleTexture() override;
    virtual GLenum GetType() override;
};