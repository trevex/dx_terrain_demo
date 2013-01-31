/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef MENUSTATE_H_
#define MENUSTATE_H_

#include "../Graphics/FontWrapper.h"


class MenuState
{
public:
	static bool update(const float &delta);
	static bool enter(void* options);
	static bool leave(void);
	static void renderGeometry(void);
	static void renderLight(void);
	static void renderForward(void);
	static void renderPost(void);
protected:
private:
	static CFontWrapper* m_font;
	static std::string m_ip;
};

#endif