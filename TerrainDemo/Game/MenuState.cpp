/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "MenuState.h"
#include "GameState.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "../State/StateManager.h"
//#include "../Network/NetworkManager.h"

#include <boost/lexical_cast.hpp>

CFontWrapper* MenuState::m_font = 0;

bool MenuState::update(const float &delta)
{
	if (KeyMap.ENTER_DOWN) 
	{
		StateManager::set<GameState>();
	}

	return true;
}

bool MenuState::enter(void* options) 
{
	std::string fontname = "Arial";
	m_font = new CFontWrapper(fontname);
	return true;
}

bool MenuState::leave(void)
{
	delete m_font;
	m_font = 0;
	return true;
}

void MenuState::renderGeometry(void)
{
	
}

void MenuState::renderLight(void)
{
	
}

void MenuState::renderForward(void)
{
	
}

void MenuState::renderPost(void)
{
	m_font->drawString("This is the Procedural Methods Coursework of Niklas K. Voss (1004462).", 22.0f, 5.0f, 5.0f);
}