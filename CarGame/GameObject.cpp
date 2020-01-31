#include "GameObject.h"


using namespace CarGame;

GameObject::GameObject() {}

GameObject::GameObject(std::string name, const myVertex vertices[], unsigned int vSize,const unsigned short indices[], unsigned int iSize) {
	this->name = name;
	this->vertices = vertices;
	this->indices = indices;
	verticesCount = vSize;
	indicesCount = iSize;
	setPosition(0.0f, 0.0f, 0.0f);
	setRotation(0.0f, 0.0f, 0.0f);
	setScale(1.0f, 1.0f, 1.0f);
}
GameObject::~GameObject(){}

//get/set properties
 void GameObject::setPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

 void GameObject::setRotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

 void GameObject::setScale(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

 XMFLOAT3 GameObject::getPosition() {
	return position;
}

 XMFLOAT3 GameObject::getRotation() {
	return rotation;
}

 XMFLOAT3 GameObject::getScale() {
	return scale;
}

 const myVertex* GameObject::getVertices() {
	return this->vertices;
}

 const unsigned short* GameObject::getIndices() {
	return this->indices;
}

 unsigned int GameObject::getIndicesCount() {
	 return indicesCount;
 }

 unsigned int GameObject::getVerticesCount() {
	 return verticesCount;
 }

XMMATRIX GameObject::getTransformMatrix() {
	// M = TRS? SRT?
	XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);

	XMMATRIX rX = XMMatrixRotationX(rotation.x);
	XMMATRIX rY = XMMatrixRotationY(rotation.y);
	XMMATRIX rZ = XMMatrixRotationZ(rotation.z);

	XMMATRIX R = rX * rY * rZ;

	XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);

	//return T*R*S;
	return S*R*T;
}

std::string GameObject::getName() {
	return name;
}

//update transform
void GameObject::Translate(XMFLOAT3 direction, float speed) {
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;
}

void GameObject::Rotate(XMFLOAT3 quaternion, float degree){
	rotation.x += quaternion.x * degree;
	rotation.y += quaternion.y * degree;
	rotation.z += quaternion.z * degree;
}