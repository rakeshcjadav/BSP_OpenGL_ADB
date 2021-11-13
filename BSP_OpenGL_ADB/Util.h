#pragma once

class CUtil
{
public:
    static std::string GetMediaPath();
    static std::string GetTexturePath();
private:
    static std::string GetEnv(std::string strName);
private:
    CUtil();
};