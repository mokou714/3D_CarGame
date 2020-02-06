#include "MouseController.h"

using namespace CarGame;

myMouse::myMouse() {
	xPos = yPos = scrollWheelValue = 0;
	leftButton = middleButton = rightButton = false;
	
}


void myMouse::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message){
	
	case WM_INPUT: {
		RAWINPUT raw;
		UINT rawSize = sizeof(raw);
		UINT resultData = GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));
		if (raw.header.dwType == RIM_TYPEMOUSE) {
			xPos = raw.data.mouse.lLastX;
			yPos = raw.data.mouse.lLastY;
		}

		return;
	}
	case WM_MOUSEWHEEL: {
		RAWINPUT raw;
		UINT rawSize = sizeof(raw);
		UINT resultData = GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));
		

		scrollWheelValue = GET_WHEEL_DELTA_WPARAM(wParam);



		return;
	}
				   
	default:
		break;
	}
}
