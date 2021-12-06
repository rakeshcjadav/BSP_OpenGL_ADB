#include"pch.h"
#include"Transform.h"

CTransform::CTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
    m_vPosition(position),
    m_vRotation(rotation),
    m_vScale(scale)
{

}

glm::mat4 CTransform::GetMatrix()
{
	// Order : Scale -> Rotate -> Translate
	glm::mat4 transformMat =
		glm::translate(glm::identity<glm::mat4>(), m_vPosition) *
		glm::toMat4(glm::quat(glm::radians(m_vRotation))) *
		glm::scale(glm::identity<glm::mat4>(), m_vScale);
	return transformMat;
}