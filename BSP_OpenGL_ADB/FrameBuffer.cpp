#include"pch.h"
#include"FrameBuffer.h"
#include"Texture.h"

CFrameBuffer::CFrameBuffer(std::string strName, int width, int height)
{
    m_strName = strName;
    glGenFramebuffers(1, &m_IDFrameBuffer);
    Bind();

    m_pColorAttachment = new CDynamicTexture("Color", GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE);
    Attach(m_pColorAttachment, GL_COLOR_ATTACHMENT0);

    m_pNormalAttachment = new CDynamicTexture("Normal", GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE);
    Attach(m_pNormalAttachment, GL_COLOR_ATTACHMENT1);

    m_pDepthAndStencilAttachment = new CDynamicTexture("Depth_Stencil", GL_DEPTH24_STENCIL8, width, height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
    Attach(m_pDepthAndStencilAttachment, GL_DEPTH_STENCIL_ATTACHMENT);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
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

void CFrameBuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBuffer::Attach(CDynamicTexture* pTexture, GLenum attachement)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, GL_TEXTURE_2D, pTexture->GetID(), 0);
}

CDynamicTexture* CFrameBuffer::GetColorAttachment()
{
    return m_pColorAttachment;
}

CDynamicTexture* CFrameBuffer::GetNormalAttachment()
{
    return m_pNormalAttachment;
}

CDynamicTexture* CFrameBuffer::GetDepthAndStencilAttachment()
{
    return m_pDepthAndStencilAttachment;
}
