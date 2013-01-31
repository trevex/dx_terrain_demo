/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef WINCOMMON_H_
#define WINCOMMON_H_

#include "RenderWindow.h"

#define WIN32_LEAN_AND_MEAN // exlude less used apis to speed up build process


// Convienence macro as seen in frank lunas source to release com interfaces
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

/* Frank Luna's DEBUG Macro for DirectX */
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#include <DxErr.h>
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 

#endif