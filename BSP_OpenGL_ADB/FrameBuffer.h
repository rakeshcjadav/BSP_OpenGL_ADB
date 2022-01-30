#pragma once

class CTexture;

class CFrameBuffer
{
public:
    CFrameBuffer(std::string strName, int width, int height, bool bMultiSample = false);
    ~CFrameBuffer();

    void Clear(glm::vec4 color);

    void Bind();
    void ReadBind();
    void DrawBind();
    void UnBind();

    void ReadBuffer(int i);
    void DrawBuffer(int i);

    void Blit();

    void Attach(CTexture* pTexture, GLenum attachement);

    CTexture* GetColorAttachment();
    CTexture* GetNormalAttachment();
    CTexture* GetDepthAndStencilAttachment();
private:
    unsigned int m_IDFrameBuffer;
    std::string m_strName;
    int m_iWidth;
    int m_iHeight;

    CTexture* m_pColorAttachment;
    CTexture* m_pNormalAttachment;
    CTexture* m_pDepthAndStencilAttachment;
};