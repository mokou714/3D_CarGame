#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//using default for initialization, no .cpp file

//parallel light
struct DirectionalLight {
	DirectionalLight() = default;
	float ambient;
	float diffuse;
	float specular;
	float padding1;
	XMFLOAT3 direction;
	float padding2;
};

struct Material {
	Material() = default;
	float ambient;
	float diffuse;
	float specular;
	float padding;
};


