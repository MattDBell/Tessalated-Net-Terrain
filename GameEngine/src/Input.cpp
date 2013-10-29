#include "Input.h"


Input * Input::instance = 0;

Input::Input()
	:keyStates()
{}
Input * Input::Get(){
	if(instance == 0)
		instance = new Input();
	return instance;
}

void Input::GainFocus(){
	keyStates.SetAllTo(false);
}
void Input::LoseFocus(){
	keyStates.SetAllTo(false);
}

void Input::UpdateWith(unsigned int key, bool to){
	keyStates[key] = to;

}
void Input::LMouseUpdate(unsigned int x, unsigned int y, bool to)
{
	mxPos = x;
	myPos = y;
	mState = to;
}
void Input::MousePosUpdate(unsigned int x, unsigned int y)
{
	mxPos = x;
	myPos = y;
}
bool Input::GetLastMousePos(int & outX, int &outY)
{
	outX = mxPos;
	outY = myPos;
	return mState;
}
bool Input::GetKey(unsigned int key){
	return bool(keyStates[key]);
}