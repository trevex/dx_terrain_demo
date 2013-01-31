/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include <Windows.h>

class CInputHandler
{
public:
	void processKeyDown(char &keyCode);
	void processKeyUp(char &keyCode);
	void processMouseUp(WPARAM &wParam);
	void processMouseDown(WPARAM &wParam);
	void processMouseMove(int x, int y);
	void cleanKeyMap(void);
protected:
private:
};

#endif