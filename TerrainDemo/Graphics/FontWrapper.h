/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef FONTWRAPPER_H_
#define FONTWRAPPER_H_


/* WINDOWS  (32bit & 64bit) */
#ifdef _WIN32 
	#ifndef WIN32_WITH_OPENGL
		#include "DirectX\DXFontWrapper.h"
		#define CFontWrapper CDXFontWrapper

	#else
		/* ... */
	#endif
	
/* LINUX */
#elif __linux__
	/* ... */

/* MAC */
#elif __APPLE__ & __MACH__
	/* ... */

#endif



#endif