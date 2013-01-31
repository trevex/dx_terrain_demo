/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm\glm.hpp>

#define Camera CCamera::getInstance()

class CCamera
{
public:
	CCamera(void);

	static CCamera* getInstance(void);

	void update(void);
	void setAspectRatio(float aspect);
	void lookAt(const glm::vec3 &position, const glm::vec3 &target = glm::vec3(0.0f, 0.0f, 0.0f));
	void setPosition(const glm::vec3 &position);
	float* getPosition(void);
	glm::vec3 getPositionVector(void) const;
	void translate(const glm::vec3 &translation);
	void rotate(const float &angle, const glm::vec3 &vector);
	glm::vec3 getDirection(void);
	float* getWorldViewProj(const glm::mat4& world = glm::mat4(1.0f));
protected:
private:

	static CCamera* m_Instance;

	glm::vec3 m_Position;
	glm::vec3 m_TargetDir;
	glm::mat4 m_View;
	glm::mat4 m_Proj;
	
	glm::mat4 m_ViewProj;
	glm::mat4 m_WorldViewProj;

	bool m_bChanged;
};

#endif