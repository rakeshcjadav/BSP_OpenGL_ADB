#pragma once

class CCamera;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/quaternion.hpp"


class CViewport
{
public:
    CViewport(glm::ivec4 size);
    void Clear();
    float GetAspectRatio();
    void SetPosition(int x, int y);
    void SetSize(int width, int height);
    void Render(CCamera* pCamera);
private:
    glm::ivec4 m_ViewportSize;
};