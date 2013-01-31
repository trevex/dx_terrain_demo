/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 *
 * TODO:
 * if time left, clean up camera: move lookAt code into update function to avoid multiple calculation triggered by look dir changed and position!
 * quaternions would also be a cleaner solution for the rotation...
 */
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CCamera* CCamera::m_Instance = 0;

const float c_Pi = 4.0f*atan(1.0f);

CCamera::CCamera(void) : m_TargetDir(1.0f), m_View(1.0f), m_Proj(1.0f), m_ViewProj(1.0f), m_WorldViewProj(1.0f), m_bChanged(false)
{

}

CCamera* CCamera::getInstance(void)
{
	if (!m_Instance)
	{
		m_Instance = new CCamera();
	}
	return m_Instance;
}

void CCamera::update(void)
{
	if (m_bChanged)
	{
		m_ViewProj = m_View*m_Proj;
		m_bChanged = false;
	}
}

void CCamera::setAspectRatio(float aspect)
{
#if defined _WIN32 && !defined WIN32_WITH_OPENGL
	float n = 1.0f; // near
	float f = 1000.0f; // far
	float a = 0.25f*3.14f; // angle
	m_Proj = glm::transpose(glm::mat4(1/(aspect * glm::tan(a / 2)), 0.0f, 0.0f, 0.0f,
										 0.0f, 1 / glm::tan(a / 2), 0.0f, 0.0f,
										 0.0f, 0.0f, f / (f - n), 1.0f,
										 0.0f, 0.0f, -n * f / (f - n), 0.0f));
#else
	m_Proj = glm::perspective(0.25f*c_Pi, aspect, 1.0f, 1000.0f);
#endif
	m_bChanged = true;
}

const glm::vec3 c_up(0.0f, 1.0f, 0.0f);
void CCamera::lookAt(const glm::vec3 &position, const glm::vec3 &target)
{
	m_Position = position;
	m_TargetDir = target - position;
#if defined _WIN32 && !defined WIN32_WITH_OPENGL
	glm::vec3 w = glm::normalize(target - position);
	glm::vec3 u = glm::normalize(glm::cross(c_up, w));
	glm::vec3 v = glm::cross(w, u);
	m_View = glm::transpose(glm::mat4(u.x, v.x, w.x, 0.0f,
									  u.y, v.y, w.y, 0.0f,
									  u.z, v.z, w.z, 0.0f,
									  -glm::dot(position, u), -glm::dot(position, v), -glm::dot(position, w), 1.0f)); 
#else
	m_View = glm::lookAt(position, target, c_up);
#endif
	m_bChanged = true;
}

float* CCamera::getWorldViewProj(const glm::mat4& world)
{
#if defined _WIN32 && !defined WIN32_WITH_OPENGL
	m_WorldViewProj = glm::transpose(glm::transpose(world) * m_ViewProj);
#else
	m_WorldViewProj = world * m_ViewProj;
#endif
	return glm::value_ptr(m_WorldViewProj);
}

void CCamera::setPosition(const glm::vec3& position)
{
	lookAt(position, position + m_TargetDir);
}

void CCamera::translate(const glm::vec3& translation)
{
	setPosition(m_Position + translation);
}

glm::vec3 CCamera::getDirection(void)
{
	return m_TargetDir;
}


void CCamera::rotate(const float &angle, const glm::vec3 &vector)
{
	glm::vec4 result = (glm::rotate(glm::mat4(1.0f), angle, vector) * glm::vec4(m_TargetDir, 0.0f));
	m_TargetDir.x = result.x;
	m_TargetDir.y = result.y;
	m_TargetDir.z = result.z;
	lookAt(m_Position, m_Position + m_TargetDir);
}

float* CCamera::getPosition(void)
{
	return glm::value_ptr(m_Position);
}


glm::vec3 CCamera::getPositionVector(void) const
{
	return m_Position;
}