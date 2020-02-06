#include "Camera.h"

using namespace CarGame;

Camera::Camera(int WindowWidth, int WindowHeight, std::shared_ptr<GameObject> target) 
	: w_width(WindowWidth), w_height(WindowHeight), target(target)
{
	
	//initialize position, looking at, up vector
	XMFLOAT3 targetPos = target->getPosition();
	setLookingAt(targetPos.x, targetPos.y, targetPos.z);
	upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	setPosition(0, 0, 0);
	
	mode = ThirdPerson;
	reset3rdView();
}
void Camera::setPosition(float x, float y, float z) {
	//store direction and distance offset to target object
	position = XMVectorSet(x,y,z,0.0f);
	dir_to_object = XMVector3Normalize(lookingAt - position);
	XMFLOAT3 length;
	XMStoreFloat3(&length, XMVector3Length(lookingAt - position));
	distant_to_object = length.x;

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


void Camera::updateTranslation() {

	//set looking up
	//XMFLOAT3 targetPos = target->getPosition();
	//lookingAt = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0);
	//translate
	/*XMFLOAT4 newPos;
	XMStoreFloat4(&newPos,position);
	newPos.x += direction.x * speed;
	newPos.y += direction.y * speed;
	newPos.z += direction.z * speed;
	position = XMLoadFloat4(&newPos);*/
	
	if (mode == ThirdPerson) {
		XMVECTOR vecToTarget = XMVector3Normalize(lookingAt-position);
		position = XMLoadFloat3(&target->getPosition()) - (vecToTarget*distant_to_object);
	}
	else {
		position = XMLoadFloat3(&target->getPosition());
	}
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
		position = lookingAt - XMVector3Transform(dir_to_object*distant_to_object, XMMatrixRotationY(target->getRotation().y));
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
		position = lookingAt - XMVector3Transform(dir_to_object*distant_to_object, XMMatrixRotationY(target->getRotation().y));
	}
}

void Camera::updateLookingAngle(float X_angleVelocity, float Y_angleVelocity) {
	if (mode == ThirdPerson) {
		XMFLOAT3 targetPos = target->getPosition();
		XMVECTOR targetPosVec = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0f);
		//set up looking at
		lookingAt = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0);
		//get vector pointing to the target
		XMVECTOR vecToTarget = targetPosVec - position;
		//rotate the vector, then use it to get the new camera position

		XMVECTOR Y_Axis = XMVectorSet(0, 1, 0, 0);
		XMVECTOR X_Axis = XMVector3Cross(vecToTarget, Y_Axis);
		XMVECTOR LocalYAxis_quaternion_rotation = XMQuaternionRotationAxis(Y_Axis , X_angleVelocity);
		XMVECTOR LocalXAxis_quaternion_rotation = XMQuaternionRotationAxis(X_Axis, Y_angleVelocity);
		XMVECTOR new_position = targetPosVec - XMVector3Rotate(XMVector3Rotate(vecToTarget,LocalYAxis_quaternion_rotation),LocalXAxis_quaternion_rotation);
		
		//limit vertical angle
		XMFLOAT3 pos_float3;
		XMStoreFloat3(&pos_float3, position);
		XMVECTOR xz_vec = XMVectorSet(targetPos.x, 0, targetPos.z, 0) - XMVectorSet(pos_float3.x,0,pos_float3.z,0) ;
		vecToTarget = targetPosVec - new_position;
		XMFLOAT3 angle;
		//calculate angle btw new looking dir and xz plane vec
		XMStoreFloat3(&angle, XMVector3AngleBetweenVectors(vecToTarget, xz_vec));
		
		//angle < 90 or > -90 degrees
		if (angle.x > -PI/2 && angle.x < PI/2 ) {
			position = new_position;
		}
	}
	else {
		//for FPS mode, do not update x axis rotation
		XMVECTOR Y_Axis = XMVectorSet(0, 1, 0, 0);
		XMVECTOR LocalYAxis_quaternion_rotation = XMQuaternionRotationAxis(Y_Axis, X_angleVelocity);
		target->Rotate(XMFLOAT3(0.0f, 1.0f, 0.0f), X_angleVelocity);
	}
}

void Camera::updateLookingDistance(float scrollWheelValue) {
	if (mode == ThirdPerson) {
		
		distant_to_object += scrollWheelValue * 0.0001f;

		if (distant_to_object < 1) { distant_to_object = 1; }
		if (distant_to_object > 10) { distant_to_object = 10; }
		updateTranslation();
	}
}




