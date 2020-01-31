#pragma once
#include <vector>
#include "GameObject.h"
#include "DirectXTex/DirectXTex.h"
#include "DirectXMesh/DirectXMesh.h"
#include "Utilities/WaveFrontReader.h"
#include <math.h>

#define PI 3.14159265358979323846

namespace CarGame {
	std::vector<std::shared_ptr<CarGame::GameObject>> LoadGameObjects();
	static myVertex* calculate_normal_from_pos_color(const VertexPosColor* vertices, const unsigned short* indices, unsigned int i_size);
	static myVertex* calculate_normal_from_pos_tex(const VertexPosTex* vertices, const unsigned short* indices, unsigned int i_size);
	static VertexPosColor* generateWheelVertices(int side_count, float height, float width);
	static unsigned short* generateWheelIndices(int side_count);
}
