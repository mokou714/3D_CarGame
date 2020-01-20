#include <DirectXMath.h>
#pragma once

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
};

struct PSConstantBuffer {
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

