#pragma once
#include "GameObject.h"
#define WHEEL_TURNING_SPEED 5.0f
#define WHEEL_ROTATING_SPEED 10.0f
#define MOVING_SPEED 9.0f
#define TURNING_SPEED 2.0f


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
		void initWheelPositions();
		void updateWheels(moving_direction dir, float dt);
		void interpolateTurningAngle(int dir, float dt);
	private:
		float velocity;
		float wheel_angle;
		float wheel_rotation;
		
	};



}
