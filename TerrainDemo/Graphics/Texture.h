/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef TEXTURE_H_
#define TEXTURE_H_


/* WINDOWS  (32bit & 64bit) */
#ifdef _WIN32 
	#ifndef WIN32_WITH_OPENGL
		#include "DirectX\DXTexture.h"
		#define CTexture CDXTexture
		#include "DirectX\DXTextureCube.h"
		#define CTextureCube CDXTextureCube
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