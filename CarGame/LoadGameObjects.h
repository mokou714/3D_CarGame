#pragma once
#include <vector>
#include "GameObject.h"
#include "DirectXTex/DirectXTex.h"
#include "DirectXMesh/DirectXMesh.h"
#include "Utilities/WaveFrontReader.h"

namespace CarGame {
	std::vector<std::shared_ptr<CarGame::GameObject>> LoadGameObjects();
	static myVertex* calculate_normal(const VertexPosColor* vertices, const unsigned short* indices, unsigned int i_size);
}
