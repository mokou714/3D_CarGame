#pragma once
#include "GameObject.h"
#define WHEEL_TURNING_SPEED 0.01
#define WHEEL_ROTATING_SPEED 0.01
#define MOVING_SPEED 0.02f


namespace CarGame {
	enum moving_direction {
		forward,
		backward,
		leftward,
		rightward
	};

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
		void updateWheels(moving_direction dir);
		void interpolateTurningAngle(int dir);
	private:
		float wheel_angle;
		float wheel_rotation;
		
	};



}
