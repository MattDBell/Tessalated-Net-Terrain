#ifndef INPUT_H
#define INPUT_H

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
	void UpdateWith(unsigned int key, bool to);
	void LMouseUpdate(unsigned int x, unsigned int y, bool up);
	void MousePosUpdate(unsigned int x, unsigned int y);
	void GainFocus();
	void LoseFocus();
	bool GetLastMousePos(int & outXPos, int & outYPos);
	bool GetKey(unsigned int key);
};


#endif