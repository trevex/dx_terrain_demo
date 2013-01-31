/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#ifndef MOUSE_H_
#define MOUSE_H_

struct SMouseMap
{
	bool L;
	bool R;
	bool M;
	int  X;
	int  Y;
	int dX;
	int dY;
	void reset(void) {
		dX = 0;
		dY = 0;
	}
};

extern SMouseMap MouseMap;

#endif