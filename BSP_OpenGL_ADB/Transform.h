#pragma once

class CTransform
{
public:
    CTransform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

    glm::mat4 GetMatrix();
private:
    glm::vec3 m_vPosition;
    glm::vec3 m_vRotation;
    glm::vec3 m_vScale;
};