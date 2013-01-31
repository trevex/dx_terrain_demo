/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "MenuState.h"
#include "GameState.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "../State/StateManager.h"
#include "../Network/NetworkManager.h"

#include <boost/lexical_cast.hpp>

CFontWrapper* MenuState::m_font = 0;
std::string MenuState::m_ip = "";

bool MenuState::update(const float &delta)
{
	if (KeyMap.ENTER_DOWN && m_ip == "") 
	{
		StateManager::set<GameState>();
	}
	else if (KeyMap.ENTER_DOWN)
	{
		if(NetworkManager->connect(const_cast<char*>(m_ip.c_str()))) StateManager::set<GameState>();
	}
	else if (KeyMap.DEL_DOWN && m_ip != "")
	{
		m_ip = m_ip.substr(0, m_ip.size()-1);
	}
	else if (KeyMap.DOT_DOWN)
	{
		m_ip += ".";
	}
	else if (KeyMap.NUMBER_DOWN[0] || KeyMap.NUMBER_DOWN[1] || KeyMap.NUMBER_DOWN[2] || KeyMap.NUMBER_DOWN[3] || KeyMap.NUMBER_DOWN[4] || KeyMap.NUMBER_DOWN[5] || KeyMap.NUMBER_DOWN[6] || KeyMap.NUMBER_DOWN[7] || KeyMap.NUMBER_DOWN[8] || KeyMap.NUMBER_DOWN[9])
	{
		for (int i = 0; i < 10; i++) if (KeyMap.NUMBER_DOWN[i]) m_ip += boost::lexical_cast<std::string>(i);
	}
	return true;
}

bool MenuState::enter(void* options) 
{
	std::string fontname = "Arial";
	m_font = new CFontWrapper(fontname);
	m_ip = "";
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
	m_font->drawString("This is the DirectX and Networking Coursework of Niklas K. Voss (1004462).", 22.0f, 5.0f, 5.0f);
	m_font->drawString("If you want to connect to a server, type in the IP and press ENTER.", 22.0f, 5.0f, 35.0f);
	m_font->drawString("If you want to simply see the scene without networking, simply press ENTER.", 22.0f, 5.0f, 65.0f);
	m_font->drawString("The controls are WASD to move and click/dragging with the Mouse to look.", 22.0f, 5.0f, 95.0f, 0xffff5555);
	m_font->drawString("IP:", 28.0f, 5.0f, 280.0f, 0xffeeeeee);
	m_font->drawString(m_ip, 28.0f, 45.0f, 280.0f, 0xffeeeeee);
}