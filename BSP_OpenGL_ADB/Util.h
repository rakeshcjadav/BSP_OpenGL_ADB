#pragma once

class CUtil
{
public:
    static std::string GetMediaPath();
    static std::string GetTexturePath();
    static std::string GetShaderPath();
    static std::string LoadShader(std::string strFileName);
private:
    static std::string GetEnv(std::string strName);
private:
    CUtil();
};