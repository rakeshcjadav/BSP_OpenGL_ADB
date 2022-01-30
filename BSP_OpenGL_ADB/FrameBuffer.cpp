#include"pch.h"
#include"FrameBuffer.h"
#include"Texture.h"

CFrameBuffer::CFrameBuffer(std::string strName, int width, int height, bool bMultiSample)
{
    m_strName = strName;
    m_iWidth = width;
    m_iHeight = height;
    glGenFramebuffers(1, &m_IDFrameBuffer);
    Bind();

    if (!bMultiSample)
        m_pColorAttachment = new CDynamicTexture("Color", GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE);
    else
        m_pColorAttachment = new CDynamicMultiSampleTexture("MSColor", GL_RGBA, width, height, 4);
    Attach(m_pColorAttachment, GL_COLOR_ATTACHMENT0);
    
    if (!bMultiSample)
        m_pNormalAttachment = new CDynamicTexture("Normal", GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE);
    else
        m_pNormalAttachment = new CDynamicMultiSampleTexture("MSNormal", GL_RGBA, width, height, 4);
    Attach(m_pNormalAttachment, GL_COLOR_ATTACHMENT1);
    
    if (!bMultiSample)
        m_pDepthAndStencilAttachment = new CDynamicTexture("Depth_Stencil", GL_DEPTH24_STENCIL8, width, height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
    else
        m_pDepthAndStencilAttachment = new CDynamicMultiSampleTexture("MSDepth_Stencil", GL_DEPTH24_STENCIL8, width, height, 4);
    Attach(m_pDepthAndStencilAttachment, GL_DEPTH_STENCIL_ATTACHMENT);
     
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer " << m_strName << " is not complete!" << std::endl;
    }

    UnBind();
}

CFrameBuffer::~CFrameBuffer()
{
    UnBind();
    glDeleteFramebuffers(1, &m_IDFrameBuffer);
}

void CFrameBuffer::Clear(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void CFrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_IDFrameBuffer);
    unsigned int attachment[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachment);
}

void CFrameBuffer::ReadBind()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_IDFrameBuffer);
}

void CFrameBuffer::DrawBind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IDFrameBuffer);
}

void CFrameBuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBuffer::ReadBuffer(int i)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
}

void CFrameBuffer::DrawBuffer(int i)
{
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
}

void CFrameBuffer::Blit()
{
    glBlitFramebuffer(0, 0, m_iWidth, m_iHeight, 0, 0, m_iWidth, m_iHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void CFrameBuffer::Attach(CTexture* pTexture, GLenum attachement)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, pTexture->GetType(), pTexture->GetID(), 0);
}

CTexture* CFrameBuffer::GetColorAttachment()
{
    return m_pColorAttachment;
}

CTexture* CFrameBuffer::GetNormalAttachment()
{
    return m_pNormalAttachment;
}

CTexture* CFrameBuffer::GetDepthAndStencilAttachment()
{
    return m_pDepthAndStencilAttachment;
}
