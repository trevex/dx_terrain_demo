/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef LIGHT_H_
#define LIGHT_H_

#include <glm\glm.hpp>

enum LIGHT_TYPE
{
	DIRECTIONALLIGHT,
	POINTLIGHT,
	SPOTLIGHT
};

class CLight
{
public:
	CLight(void);

	void render(void);

	LIGHT_TYPE Type;
	glm::vec3 LightPosition; 
	glm::vec3 LightColor; 
	glm::vec3 LightDirection; 
	glm::vec2 SpotlightAngles; 
	glm::vec4 LightRange;
protected:
private:
};

#endif