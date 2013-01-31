/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 * Very simple "PlayerController" - Camera Controller
 * basically a simplified version of the FreeCamera with the ability to attach an object that is moved around
 */

#ifndef PLAYERCONTROLLER_H_
#define PLAYERCONTROLLER_H_

#include "../Graphics/Camera.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"

// TODO: clean up and put global variables in a scope
static const float mov_factor = 0.0009f;
static const float look_factor = 0.5f;
static const glm::vec3 up_vector(0.0f, 1.0f, 0.0f);
static const glm::vec3 left_vector(1.0f, 0.0f, 0.0f);

template<class MeshType>
class CPlayerController
{
public:
	CPlayerController(void) : m_actor(0), m_angle(0.0f)
	{
		Camera->lookAt(glm::vec3(5.0f, 2.0f, 5.0f));
	}

	void update(const float &delta)
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
		if (MouseMap.L)
		{
			float angle = -look_factor * MouseMap.dX;
			Camera->rotate(angle, up_vector);
			m_actor->rotate(angle, up_vector);
			m_angle += angle;
		}
		if (mov.x || mov.y || mov.z) {
			Camera->translate(mov);
			m_actor->translate(mov);
		}
	}

	void attach(MeshType* actor)
	{
		m_actor = actor;
	}
protected:
private:
	MeshType* m_actor;

	float m_angle;
};

#endif