#pragma once
#include "GameObject.h"

namespace CarGame {
	struct Wheels {
		GameObject* leftFrontWheel;
		GameObject* rightFrontWheel;
		GameObject* leftRearWheel;
		GameObject* rightRearWheel;
		~Wheels() {
			delete leftFrontWheel;
			delete rightFrontWheel;
			delete leftRearWheel;
			delete rightRearWheel;
		}
	};

	class Car : public GameObject {
	public:
		Car(std::string name, const myVertex vertices[], unsigned int vSize, const unsigned short indices[], unsigned int iSize,
			Wheels wheels);
		~Car();
	private:
		Wheels* wheels;
	};



}
