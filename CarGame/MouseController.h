#pragma once
#include <windows.h>
#include <memory>

namespace CarGame {

	class myMouse {
		
	public:

		bool leftButton;
		bool middleButton;
		bool rightButton;
		int xPos;
		int yPos;
		int scrollWheelValue;
		HWND m_MainWindow;

		myMouse(HWND window);

		//handle window message, update current state
		void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	};



}