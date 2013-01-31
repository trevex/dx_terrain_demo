/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 *
 * Very simple "FreeCam" - Camera Controller
 */

#include "FreeCamera.h"

#include "../Graphics/Camera.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"

const float mov_factor = 0.005f;
const float look_factor = 0.3f;
const glm::vec3 up_vector(0.0f, 1.0f, 0.0f);
const glm::vec3 left_vector(1.0f, 0.0f, 0.0f);

CFreeCamera::CFreeCamera(void)
{
	Camera->lookAt(glm::vec3(5.0f, 2.0f, 5.0f));
}

void CFreeCamera::update(const float &delta)
{
	glm::vec3 dir = Camera->getDirection();
	glm::vec3 mov(0.0f);
	if (KeyMap.A) 
	{
		mov.x += -dir.z * mov_factor * +delta;
		mov.z += +dir.x * mov_factor * +delta;
	}
	else if (KeyMap.D)
	{
		mov.x += -dir.z * mov_factor * -delta;
		mov.z += +dir.x * mov_factor * -delta;
	}
	if (KeyMap.W) 
	{
		mov.x += dir.x * mov_factor * +delta;
		mov.z += dir.z * mov_factor * +delta;
	}
	else if (KeyMap.S) 
	{
		mov.x += dir.x * mov_factor * -delta;
		mov.z += dir.z * mov_factor * -delta;
	}
	if (KeyMap.SPACE)
	{
		mov.y += glm::sqrt(dir.x * dir.x + dir.z * dir.z) * mov_factor * delta;
	}
	else if (KeyMap.C)
	{
		mov.y += glm::sqrt(dir.x * dir.x + dir.z * dir.z) * mov_factor * -delta;
	}

	if (MouseMap.L)
	{
		Camera->rotate(-look_factor * MouseMap.dX, up_vector);
		Camera->rotate(+look_factor * MouseMap.dY, left_vector);
	}
	if (mov.x || mov.y || mov.z) Camera->translate(mov);
}