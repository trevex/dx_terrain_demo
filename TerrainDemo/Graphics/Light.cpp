/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "Light.h"


CLight::CLight(void) : Type(DIRECTIONALLIGHT), LightPosition(0.0f, 0.0f, 0.0f), LightColor(0.3f, 0.3f, 0.3f), LightDirection(-1.0f, -2.0f, 1.0f), SpotlightAngles(1.0f, 1.0f), LightRange(1.0f, 1.0f, 1.0f, 1.0f)
{

}

/* WINDOWS  (32bit & 64bit) */
#ifdef _WIN32 
#ifndef WIN32_WITH_OPENGL
	
#include "DirectX\DXLightingShader.h"
void CLight::render(void) 
{
	DXLightingShader->set(*this);
	DXLightingShader->renderCurrent();
}

#else
#endif
/* LINUX */
#elif __linux__
/* MAC */
#elif __APPLE__ & __MACH__
#endif