#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//using default for initialization, no .cpp file

//parallel light
struct DirectionalLight {
	DirectionalLight() = default;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 direction;
};

struct Material {
	Material() = default;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 reflect;
};


