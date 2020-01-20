#include "Camera.h"

using namespace CarGame;

Camera::Camera(int WindowWidth, int WindowHeight, std::shared_ptr<GameObject> target) 
	: w_width(WindowWidth), w_height(WindowHeight), target(target)
{
	position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT3 targetPos = target->getPosition();
	lookingAt = XMVectorSet(targetPos.x,targetPos.y,targetPos.z,0.0);
	upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	default_3rd_pos_offset = lookingAt;
	
	mode = ThirdPerson;
	reset3rdView();
}
void Camera::setPosition(float x, float y, float z) {
	position = XMVectorSet(x,y,z,0.0f);
	default_3rd_pos_offset = lookingAt - position;
	reset3rdView();
}
void Camera::setLookingAt(float x, float y, float z) {
	lookingAt = XMVectorSet(x,y,z,0.0f);
}

XMVECTOR Camera::getPosition() {
	return position;
}
XMVECTOR Camera::getLookingAt() {
	return lookingAt;
}

float Camera::getAspectRatio() {
	return (float)w_width / w_height;
}


void Camera::updateTranslation(XMFLOAT3 direction, float speed) {
	//set looking up
	//XMFLOAT3 targetPos = target->getPosition();
	//lookingAt = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0);
	//translate
	XMFLOAT4 newPos;
	XMStoreFloat4(&newPos,position);
	newPos.x += direction.x * speed;
	newPos.y += direction.y * speed;
	newPos.z += direction.z * speed;
	position = XMLoadFloat4(&newPos);
}

void Camera::switchCamMode() {
	if (mode == FirstPerson)
		mode = ThirdPerson;
	else
		mode = FirstPerson;

	//set up 3rd person view
	if (mode == ThirdPerson) {
		XMFLOAT3 targetPos = target->getPosition();
		lookingAt = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0);
		position = lookingAt - XMVector3Transform(default_3rd_pos_offset, XMMatrixRotationY(target->getRotation().y));
	}
	//set up 1st person view
	else {
		//set looking up(look at a distant point)
		XMVECTOR farPoint = XMVectorSet(0.0f, 0.0f, 100.0f, 1.0f);
		lookingAt = XMVector3Transform(farPoint, XMMatrixRotationY(target->getRotation().y));
		//camera and target have the same position
		XMFLOAT3 targetPos = target->getPosition();
		position = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0f);
	}
}

void Camera::reset3rdView() {
	if (mode == ThirdPerson) {
		XMFLOAT3 targetPos = target->getPosition();
		lookingAt = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0);
		position = lookingAt - XMVector3Transform(default_3rd_pos_offset, XMMatrixRotationY(target->getRotation().y));
	}
}

void Camera::updateHorizontal(float angleVelocity) {
	if (mode == ThirdPerson) {
		XMFLOAT3 targetPos = target->getPosition();
		XMVECTOR targetPosVec = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0f);
		//set up looking at
		lookingAt = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0);
		//get vector pointing to the target
		XMVECTOR vecToTarget = targetPosVec - position;
		//rotate the vector, then use it to get the new camera position
		position = targetPosVec - XMVector4Transform(vecToTarget, XMMatrixRotationY(angleVelocity));
	}
	else {
		//this is for A/D camera control
		//set looking up(look at a distant point)
		//XMVECTOR farPoint = XMVectorSet(0.0f, 0.0f, 100.0f, 1.0f);
		//lookingAt = XMVector3Transform(farPoint, XMMatrixRotationY(target->getRotation().y));

		lookingAt = XMVector3Transform(lookingAt, XMMatrixRotationY(angleVelocity));
		target->Rotate(XMFLOAT3(0.0f, 1.0f, 0.0f), angleVelocity);
	}
}

void Camera::updateVertical(float angleVelocity) {
	if (mode == ThirdPerson) {
		XMFLOAT3 targetPos = target->getPosition();
		XMVECTOR targetPosVec = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0f);
		//set up looking at
		lookingAt = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0);
		//get vector pointing to the target
		XMVECTOR vecToTarget = targetPosVec - position;
		//rotate the vector, then use it to get the new camera position
		position = targetPosVec - XMVector4Transform(vecToTarget, XMMatrixRotationX(angleVelocity));
	}
	else {
		//this is for A/D camera control
		//set looking up(look at a distant point)
		//XMVECTOR farPoint = XMVectorSet(0.0f, 0.0f, 100.0f, 1.0f);
		//lookingAt = XMVector3Transform(farPoint, XMMatrixRotationX(target->getRotation().y));

		//First person mode, camera does not move up and down
		//lookingAt = XMVector3Transform(lookingAt, XMMatrixRotationX(angleVelocity));
		//target->Rotate(XMFLOAT3(0.0f, 0.0f, 1.0f), angleVelocity);
	}
}
