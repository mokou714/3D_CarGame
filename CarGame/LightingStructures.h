#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//parallel light
struct DirectionalLight {
	DirectionalLight();
	DirectionalLight(const XMFLOAT4& ambient, const XMFLOAT4 diffuse, const XMFLOAT4 specular, const XMFLOAT3 direction);
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 direction;
};

struct Material {
	Material();
	Material(const XMFLOAT4& ambient, const XMFLOAT4 diffuse, const XMFLOAT4 specular, const XMFLOAT3 direction);
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 direction;
};


