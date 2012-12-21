#include "Input.h"
#include <Windowsx.h>

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
	PBYTE arr = (PBYTE)malloc(sizeof(BYTE) * 256);
	if(GetKeyboardState(arr)){
		for(size_t i = 0; i < 256; ++i)
			keyStates[i] = (arr[i]>> 7 )>0;
	}
}
void Input::LoseFocus(){
	keyStates.SetAllTo(false);
}

void Input::UpdateWith(WPARAM key, bool to){
	if(key >= 0 && key <256)
		keyStates[key] = to;

}
void Input::LMouseUpdate(LPARAM position, bool to)
{
	mxPos = GET_X_LPARAM(position);
	myPos = GET_Y_LPARAM(position);
	mState = to;
}
void Input::MousePosUpdate(LPARAM position)
{
	mxPos = GET_X_LPARAM(position);
	myPos = GET_Y_LPARAM(position);
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