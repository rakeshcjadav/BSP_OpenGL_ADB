#pragma once

class CTexture
{
public:
    CTexture(std::string strFileName);
    void Bind(unsigned int index);
private:
    unsigned int LoadTexture(std::string strFileName);
private:
    unsigned int m_IDTexture;
};