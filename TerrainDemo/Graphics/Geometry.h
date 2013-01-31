/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef GEOMETRY_H_
#define GEOMETRY_H_


/* WINDOWS  (32bit & 64bit) */
#ifdef _WIN32 
	#ifndef WIN32_WITH_OPENGL
		#include "DirectX\DXCubeGeometry.h"
		#define CCubeGeometry CDXCubeGeometry
		#include "DirectX\DXQuadGeometry.h"
		#define CQuadGeometry CDXQuadGeometry
		#include "DirectX\DXSkySphereGeometry.h"
		#define CSkySphereGeometry CDXSkySphereGeometry


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