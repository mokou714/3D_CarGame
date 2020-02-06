#pragma once
#include <DirectXMath.h>
#include <string>
#include <algorithm>
#include "ShaderStructure.h"
#define PI 3.14159265358979323846

using namespace DirectX;

namespace CarGame {
	class GameObject {
	public:
		GameObject();
		GameObject(std::string name, const myVertex vertices[], unsigned int vSize, const unsigned short indices[], unsigned int iSize);
		virtual ~GameObject();

		virtual XMFLOAT3 getPosition();
		virtual XMFLOAT3 getRotation();
		virtual XMFLOAT3 getScale();
		virtual const myVertex* getVertices();
		virtual const unsigned short* getIndices();
		virtual unsigned int getIndicesCount();
		virtual unsigned int getVerticesCount();
		virtual std::string getName();
		virtual void setPosition(float x, float y, float z);
		virtual void setRotation(float x, float y, float z);
		virtual void setScale(float x, float y, float z);
		virtual XMMATRIX getTransformMatrix();
		virtual void Translate(XMFLOAT3 direction, float speed);
		virtual void Rotate(XMFLOAT3 axis, float degree);

		GameObject* parent;

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