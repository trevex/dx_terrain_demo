/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "InputHandler.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"

void CInputHandler::processKeyDown(char &keyCode)
{
	switch (keyCode)
    {
	case 13: // Enter
		KeyMap.ENTER = true;
		KeyMap.ENTER_DOWN = true;
		break;
	case 27: // Escape
		KeyMap.ESC = true;
		break;
	case 32: // Space
		KeyMap.SPACE = true;
		break;
	case 8: // DEL
		KeyMap.DEL = true;
		KeyMap.DEL_DOWN = true;
		break;
	case 'W':
		KeyMap.W = true;
		break;
    case 'A':
		KeyMap.A = true;
		break;
	case 'S':
		KeyMap.S = true;
		break;
	case 'D':
		KeyMap.D = true;
		break;
	case 'C':
		KeyMap.C = true;
		break;
	case '.':
		KeyMap.DOT = true;
		KeyMap.DOT_DOWN = true;
		break;
	case '0':
		KeyMap.NUMBER[0] = true;
		KeyMap.NUMBER_DOWN[0] = true;
		break;
	case '1':
		KeyMap.NUMBER[1] = true;
		KeyMap.NUMBER_DOWN[1] = true;
		break;
	case '2':
		KeyMap.NUMBER[2] = true;
		KeyMap.NUMBER_DOWN[2] = true;
		break;
	case '3':
		KeyMap.NUMBER[3] = true;
		KeyMap.NUMBER_DOWN[3] = true;
		break;
	case '4':
		KeyMap.NUMBER[4] = true;
		KeyMap.NUMBER_DOWN[4] = true;
		break;
	case '5':
		KeyMap.NUMBER[5] = true;
		KeyMap.NUMBER_DOWN[5] = true;
		break;
	case '6':
		KeyMap.NUMBER[6] = true;
		KeyMap.NUMBER_DOWN[6] = true;
		break;
	case '7':
		KeyMap.NUMBER[7] = true;
		KeyMap.NUMBER_DOWN[7] = true;
		break;
	case '8':
		KeyMap.NUMBER[8] = true;
		KeyMap.NUMBER_DOWN[8] = true;
		break;
	case '9':
		KeyMap.NUMBER[9] = true;
		KeyMap.NUMBER_DOWN[9] = true;
		break;
	default:
		break;
	}
}

void CInputHandler::processKeyUp(char &keyCode)
{
	switch (keyCode)
    {
	case 13: // Enter
		KeyMap.ENTER = false;
		break;
	case 27: // Escape
		KeyMap.ESC = false;
		break;
	case 32: // Space
		KeyMap.SPACE = false;
		break;
	case 8: // DEL
		KeyMap.DEL = false;
		break;
	case 'W':
		KeyMap.W = false;
		break;
    case 'A':
		KeyMap.A = false;
		break;
	case 'S':
		KeyMap.S = false;
		break;
	case 'D':
		KeyMap.D = false;
		break;
	case 'C':
		KeyMap.C = false;
		break;
	case '.':
		KeyMap.DOT = false;
		break;
	case '0':
		KeyMap.NUMBER[0] = false;
		break;
	case '1':
		KeyMap.NUMBER[1] = false;
		break;
	case '2':
		KeyMap.NUMBER[2] = false;
		break;
	case '3':
		KeyMap.NUMBER[3] = false;
		break;
	case '4':
		KeyMap.NUMBER[4] = false;
		break;
	case '5':
		KeyMap.NUMBER[5] = false;
		break;
	case '6':
		KeyMap.NUMBER[6] = false;
		break;
	case '7':
		KeyMap.NUMBER[7] = false;
		break;
	case '8':
		KeyMap.NUMBER[8] = false;
		break;
	case '9':
		KeyMap.NUMBER[9] = false;
		break;
	default:
		break;
	}
}

void CInputHandler::processMouseUp(WPARAM &wParam)
{
	if((wParam & MK_LBUTTON) != 0)
	{
		MouseMap.L = false;
	}
	else if((wParam & MK_RBUTTON) != 0)
	{
		MouseMap.R = false;
	}
	else if ((wParam & MK_MBUTTON) != 0)
	{
		MouseMap.M = false;
	}
}

void CInputHandler::processMouseDown(WPARAM &wParam)
{
	if((wParam & MK_LBUTTON) != 0)
	{
		MouseMap.L = true;
	}
	else if((wParam & MK_RBUTTON) != 0)
	{
		MouseMap.R = true;
	}
	else if ((wParam & MK_MBUTTON) != 0)
	{
		MouseMap.M = true;
	}
}

void CInputHandler::processMouseMove(int x, int y)
{
	MouseMap.dX = MouseMap.X - x;
	MouseMap.dY = MouseMap.Y - y;
	MouseMap.X = x;
	MouseMap.Y = y;
}

void CInputHandler::cleanKeyMap(void)
{
	KeyMap.DEL_DOWN = false;
	KeyMap.DOT_DOWN = false;
	KeyMap.ENTER_DOWN = false;
	for (int i = 0; i < 10; i++) KeyMap.NUMBER_DOWN[i] = false;
}