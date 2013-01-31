/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

struct SKeyMap
{
	bool W;
	bool A;
	bool S;
	bool D;
	bool ENTER;
	bool SPACE;
	bool Q;
	bool E;
	bool R;
	bool F;
	bool ESC;
	bool C;
	bool DOT;
	bool DEL;
	bool NUMBER[10];
	bool DOT_DOWN;
	bool DEL_DOWN;
	bool ENTER_DOWN;
	bool NUMBER_DOWN[10];
};

extern SKeyMap KeyMap;

#endif