/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#include "DXFontWrapper.h"
#include "../../Common.h"
#include "../RenderInfo.h"
#include "DXRenderInfo.h"

bool CDXFontWrapper::m_bFactoryActive = false;
IFW1Factory* CDXFontWrapper::m_pFactory = 0;

CDXFontWrapper::CDXFontWrapper(std::string &fontname) : m_pFont(0)
{
	if (!m_bFactoryActive) 
	{
		HR(FW1CreateFactory(FW1_VERSION, &m_pFactory)); 
		m_bFactoryActive = true;
	}

	std::wstring tmp(fontname.begin(), fontname.end());

	HR(m_pFactory->CreateFontWrapper(static_cast<SDXRenderInfo*>(g_RenderInfo)->device, tmp.c_str(), &m_pFont));
}

CDXFontWrapper::~CDXFontWrapper(void)
{
	m_pFont->Release();
}

void CDXFontWrapper::drawString(std::string text, float size, float x, float y, unsigned int abgr)
{
	m_pFont->DrawString(
		static_cast<SDXRenderInfo*>(g_RenderInfo)->context,
		std::wstring(text.begin(), text.end()).c_str(),
		size,
		x,
		y,
		abgr,
		FW1_RESTORESTATE
	);
}