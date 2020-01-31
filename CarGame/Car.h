#pragma once
#include "GameObject.h"
#define PI 3.14159265358979323846

namespace CarGame {
	struct Wheels {
		GameObject* leftFrontWheel;
		GameObject* rightFrontWheel;
		GameObject* leftRearWheel;
		GameObject* rightRearWheel;
	};

	class Car : public GameObject {
	public:
		Car(std::string name, const myVertex vertices[], unsigned int vSize, const unsigned short indices[], unsigned int iSize);
		Wheels wheels;
		//update wheels
		void initWheelPositions();
		void updateWheels(int dir);
	};



}
