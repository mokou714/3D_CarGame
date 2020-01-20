#pragma once
#include <DirectXMath.h>
#include <string>
#include <algorithm>
#include "ShaderStructure.h"

using namespace DirectX;

namespace CarGame {
	class GameObject {
	public:
		GameObject();
		GameObject(std::string name, const myVertex vertices[], unsigned int vSize, const unsigned short indices[], unsigned int iSize);
		~GameObject();

		XMFLOAT3 getPosition();
		XMFLOAT3 getRotation();
		XMFLOAT3 getScale();
		const myVertex* getVertices();
		const unsigned short* getIndices();
		unsigned int getIndicesCount();
		unsigned int getVerticesCount();
		std::string getName();

		void setPosition(float x, float y, float z);
		void setRotation(float x, float y, float z);
		void setScale(float x, float y, float z);

		XMMATRIX getTransformMatrix();

		void Translate(XMFLOAT3 direction, float speed);
		void Rotate(XMFLOAT3 axis, float degree);

	private:
		std::string name;
		const myVertex* vertices; //local space positions
		const unsigned short* indices;
		unsigned int indicesCount;
		unsigned int verticesCount;
		XMFLOAT3 position;
		XMFLOAT3 rotation; //in degrees
		XMFLOAT3 scale;

	};
}