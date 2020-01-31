#include "Car.h"

using namespace CarGame;

Car::Car(std::string name, const myVertex vertices[], unsigned int vSize, const unsigned short indices[], unsigned int iSize)
	:GameObject(name,vertices,vSize,indices,iSize)
{}

void Car::initWheelPositions() {
	XMFLOAT3 carPos = getPosition();
	float xOffset = 0.35f;
	float yOffset = 0.30f;
	float zOffset = 0.35f;
	wheels.leftFrontWheel->setPosition(carPos.x - xOffset, carPos.y - yOffset, carPos.z + zOffset);
	wheels.rightFrontWheel->setPosition(carPos.x + xOffset, carPos.y - yOffset, carPos.z + zOffset);
	wheels.leftRearWheel->setPosition(carPos.x - xOffset, carPos.y - yOffset, carPos.z - zOffset);
	wheels.rightRearWheel->setPosition(carPos.x + xOffset, carPos.y - yOffset, carPos.z - zOffset);
}


void Car::updateWheels(int dir) {
	XMFLOAT3 carRot = getRotation();
	XMFLOAT3 carScale = getScale();
	//update rotations
	float rotateAngle = PI / 6;
	if (dir == 0) {
		wheels.leftFrontWheel->setRotation(carRot.x, 0, carRot.z);
		wheels.rightFrontWheel->setRotation(carRot.x, 0, carRot.z);
	}
	else {
		wheels.leftFrontWheel->setRotation(carRot.x, rotateAngle*dir, carRot.z);
		wheels.rightFrontWheel->setRotation(carRot.x, rotateAngle*dir, carRot.z);
	}
	//wheels.leftRearWheel->setRotation(carRot.x, carRot.y, carRot.z);
	//wheels.rightRearWheel->setRotation(carRot.x, carRot.y, carRot.z);

}



