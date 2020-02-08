#include "KeyboardController.h"

using namespace CarGame;

myKeyboard::myKeyboard(){}

void myKeyboard::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	bool keyDown = false;

	switch (message)
	{
	case WM_ACTIVATEAPP:
		ResetState();
		return;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		keyDown = true;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		break;
	default:
		return; //unknown message
	}

	int key_value = static_cast<int>(wParam);

	switch (key_value) {
	case 0x57: //W
		state.W = keyDown ? true : false;
		return;
	case 0x53: //S
		state.S = keyDown ? true : false;
		return;
	case 0x41: //A
		state.A = keyDown ? true : false;
		return;
	case 0x44://D
		state.D = keyDown ? true : false;
		return;
	case 0x52://R
		state.R = keyDown ? true : false;
		return;
	case 0x46://F
		state.F = keyDown ? true : false;
		return;

	default:
		return;
	}
}

void myKeyboard::ResetState() {
	state.A = state.D = state.W = state.S = 
		state.LeftArrow = state.RightArrow = state.UpArrow = state.DownArrow = 
		state.LShift = state.RShift = state.R = state.F = false;
}


