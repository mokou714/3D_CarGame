#pragma once
#include <windows.h>
#include <memory>

namespace CarGame {
	
	struct MouseInfo
	{
		bool leftButton;
		bool middleButton;
		bool rightButton;
		int xPos;
		int yPos;
		int scrollWheelValue;
	};


	//update mouse state for each frame
	class myMouseState {
	public:
		enum ButtonState{
			up,
			down,
			buttonUp, //when btn released
			buttonDown //when btn pressed
		};

		ButtonState leftButton;
		ButtonState middleButton;
		ButtonState rightButton;

		myMouseState();

		void Update(const MouseInfo& info);

		void Reset() ;

		MouseInfo GetLastMouseInfo() const { 
			return lastMouseInfo; 
		}
	private:
		MouseInfo lastMouseInfo;
		MouseInfo GetMouseInfo() const;
		void  ResetScrollWheelValue();
		void SetVisible(bool visible); // Cursor visibility

	};





}