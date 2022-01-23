#include"pch.h"
#include"Texture.h"

// CTexture
CTexture::CTexture(std::string strName)
{
    m_strName = strName;
}

void CTexture::Bind(unsigned int index)
{
    glActiveTexture(GL_TEXTURE0 + index); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, m_IDTexture);
}

unsigned int CTexture::GetID()
{
    return m_IDTexture;
}

CTexture::~CTexture()
{

}

// CFileTexture
CFileTexture::CFileTexture(std::string strFileName):
    CTexture(strFileName)
{
    m_IDTexture = LoadTexture(strFileName);
}

CFileTexture::~CFileTexture()
{

}

unsigned int CFileTexture::LoadTexture(std::string strFileName)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(strFileName.c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::cout << "Failed to load the image :" << strFileName << std::endl;
        return 0;
    }

    std::cout << "Image Name : " << strFileName << std::endl;
    std::cout << "Image Width : " << width << std::endl;
    std::cout << "Channels : " << nrChannels << std::endl;

    GLint format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Filtering ( Tri-linear filtering )
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}


// CDynamicTexture
CDynamicTexture::CDynamicTexture(std::string strName, GLint internalformat, GLsizei width, GLsizei height, GLint format, GLenum type):
    CTexture(strName)
{
    glGenTextures(1, &m_IDTexture);
    glBindTexture(GL_TEXTURE_2D, m_IDTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

CDynamicTexture::~CDynamicTexture()
{

}
