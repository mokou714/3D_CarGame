#pragma once
#include <DirectXMath.h>
#include "LightingStructures.h"

struct myVertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 normal;
};

struct VertexPosColor {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};

struct VSConstantBuffer {
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX inv_world_view;
};

struct PSConstantBuffer {
	DirectionalLight dirLight; //60bytes
	Material material; //60bytes
	DirectX::XMFLOAT3 eyePos; //12bytes
	DirectX::XMFLOAT3 offset; //132->144(16*9)
};

