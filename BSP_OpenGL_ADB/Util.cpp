#include"pch.h"
#include"Util.h"

std::string CUtil::GetEnv(std::string strName)
{
    char* libvar;
    size_t requiredSize;
    getenv_s(&requiredSize, nullptr, 0, strName.c_str());
    if (requiredSize == 0)
    {
        //   LogMessage(fmt::format("Environment variable {0} doesn't exist!", varName).c_str());
        return "";
    }

    libvar = (char*)malloc(requiredSize * sizeof(char));
    if (!libvar)
    {
        //   LogMessage(fmt::format("Failed to allocate memory {0} bytes!", requiredSize).c_str());
        return "";
    }

    // Get the value of the LIB environment variable.
    getenv_s(&requiredSize, libvar, requiredSize, strName.c_str());

    return libvar;
}

std::string CUtil::GetMediaPath()
{
    return GetEnv("MEDIA_PATH") + "\\";
}

std::string CUtil::GetTexturePath()
{
    return GetMediaPath() + "textures\\";
}