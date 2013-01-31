/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef DXFONTWRAPPER_H_
#define DXFONTWRAPPER_H_

#include <FW1FontWrapper.h>
#include <string>


class CDXFontWrapper
{
public:
	CDXFontWrapper(std::string &fontname);
	~CDXFontWrapper(void);

	void drawString(std::string text, float size, float x, float y, unsigned int abgr = 0xff0099ff);
protected:
private:
	static IFW1Factory* m_pFactory;
	static bool m_bFactoryActive;

	IFW1FontWrapper* m_pFont;
};

#endif