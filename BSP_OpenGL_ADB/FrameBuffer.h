#pragma once

class CDynamicTexture;

class CFrameBuffer
{
public:
    CFrameBuffer(std::string strName, int width, int height);
    ~CFrameBuffer();

    void Clear(glm::vec4 color);

    void Bind();
    void UnBind();

    void Attach(CDynamicTexture* pTexture, GLenum attachement);

    CDynamicTexture* GetColorAttachment();
    CDynamicTexture* GetNormalAttachment();
    CDynamicTexture* GetDepthAndStencilAttachment();
private:
    unsigned int m_IDFrameBuffer;
    std::string m_strName;

    CDynamicTexture* m_pColorAttachment;
    CDynamicTexture* m_pNormalAttachment;
    CDynamicTexture* m_pDepthAndStencilAttachment;
};