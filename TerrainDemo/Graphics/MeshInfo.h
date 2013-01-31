/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef MESHINFO_H_
#define MESHINFO_H_

#include <glm\glm.hpp>

struct SMeshInfo
{
	glm::mat4 world;
	glm::vec3 position;
	glm::mat4 rotation;
};

#endif