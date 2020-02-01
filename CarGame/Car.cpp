#include "Car.h"

using namespace CarGame;

Car::Car(std::string name, const myVertex vertices[], unsigned int vSize, const unsigned short indices[], unsigned int iSize)
	:GameObject(name,vertices,vSize,indices,iSize), wheel_angle(0.0f),wheel_rotation(0.0f)
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


void Car::updateWheels(moving_direction dir) {
	XMFLOAT3 carRot = getRotation();
	XMFLOAT3 carScale = getScale();
	//update rotations
	float rotateAngle = PI / 6;
	if (dir == forward or dir == backward) {
		int _dir = dir == forward ? 1 : -1;
		wheel_rotation = fmod(wheel_rotation + WHEEL_ROTATING_SPEED * _dir, 2 * PI);
		wheels.leftFrontWheel->setRotation(wheel_rotation, 0, carRot.z);
		wheels.rightFrontWheel->setRotation(wheel_rotation, 0, carRot.z);
		wheels.leftRearWheel->setRotation(wheel_rotation, 0, carRot.z);
		wheels.rightRearWheel->setRotation(wheel_rotation, 0, carRot.z);
	}
	else {
		int _dir = dir == leftward ? -1 : 1;
		wheel_angle += WHEEL_TURNING_SPEED * _dir;
		if (wheel_angle > PI / 6)
			wheel_angle = PI / 6;
		if (wheel_angle < -PI / 6)
			wheel_angle = -PI / 6;
		wheels.leftFrontWheel->setRotation(wheel_rotation, wheel_angle, carRot.z);
		wheels.rightFrontWheel->setRotation(wheel_rotation, wheel_angle, carRot.z);
	}
	//wheels.leftRearWheel->setRotation(carRot.x, carRot.y, carRot.z);
	//wheels.rightRearWheel->setRotation(carRot.x, carRot.y, carRot.z);

}



