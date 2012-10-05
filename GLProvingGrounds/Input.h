#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include "BoolArray.h"

class Input{
	static const unsigned int NUMKEYS = 0xFF;
	BoolArray<NUMKEYS> keyStates;
	static Input * instance;
	Input();
public:
	static Input* Get();

	void UpdateWith(WPARAM msg, bool up);
	void GainFocus();
	void LoseFocus();
	bool GetKey(unsigned int key);
};


#endif