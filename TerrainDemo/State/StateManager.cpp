/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#include "StateManager.h"
#include "../Input/Mouse.h"

StateManager::funcUpdateType StateManager::m_funcUpdate = 0;
StateManager::funcLeaveType StateManager::m_funcLeave = 0;
StateManager::funcRenderType StateManager::m_funcGeom = 0;
StateManager::funcRenderType StateManager::m_funcLight = 0;
StateManager::funcRenderType StateManager::m_funcPost = 0;
StateManager::funcRenderType StateManager::m_funcForward = 0;

bool StateManager::update(const float &delta)
{
	bool result = m_funcUpdate(delta);
	MouseMap.reset();
	return result;
}

void StateManager::renderGeometry(void)
{
	m_funcGeom();
}

void StateManager::renderLight(void)
{
	m_funcLight();
}

void StateManager::renderForward(void)
{
	m_funcForward();
}

void StateManager::renderPost(void)
{
	m_funcPost();
}
