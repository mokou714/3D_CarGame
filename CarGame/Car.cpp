#include "Car.h"

using namespace CarGame;
Car::Car(std::string name, const myVertex vertices[], unsigned int vSize, const unsigned short indices[], unsigned int iSize,
	Wheels wheels): GameObject(name, vertices, vSize, indices, iSize),wheels(&wheels)
{}

Car::~Car() {
	delete wheels;
}


