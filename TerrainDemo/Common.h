/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 *
 * The Common framework is platform dependent along with the programs-main-function.
 * Although DirectX is used to fullfil the Coursework requirements, the application will
 * use a form of abstraction to make it easily portable and flexible enought to plug-in
 * different sub-system (e.g. OpenGL, Gtk, OpenAL)
 */

#ifndef COMMON_H_
#define COMMON_H_

#define GLM_FORCE_RADIANS

/* platform-specif pre-defines: http://sourceforge.net/p/predef/wiki/OperatingSystems/ */

/*
 * WINDOWS  (32bit & 64bit)
 */
#ifdef _WIN32 
	#include "Win/Common.h"
	#define MAINENTRY int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
	
/*
 * LINUX
 */
#elif __linux__
	#include "Linux/Common.h"
	#define MAINENTRY int main(int argc, char* argv[])
	

/*
 * MAC
 */
#elif __APPLE__ & __MACH__
	#include "Mac/Common.h"
	#define MAINENTRY int main(int argc, char* argv[])
	
#endif

#include "Graphics\Renderer.h"

#endif