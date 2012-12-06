#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include "BoolArray.h"

class Input{
	static const unsigned int NUMKEYS = 0xFF;
	BoolArray<NUMKEYS> keyStates;
	static Input * instance;
	Input();
	int mxPos, myPos;
	bool mState;
public:
	static Input* Get();
	void UpdateWith(WPARAM msg, bool up);
	void LMouseUpdate(LPARAM msg, bool up);
	void MousePosUpdate(LPARAM msg);
	void GainFocus();
	void LoseFocus();
	bool GetLastMousePos(int & outXPos, int & outYPos);
	bool GetKey(unsigned int key);
};


#endif