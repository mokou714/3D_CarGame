#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "d3dApp.h"
#include "Utilities.h"
#include "LightingStructures.h"

namespace CarGame {
	class ShadowRenderer {

	public:
		ShadowRenderer();

		void initResources();



		//d3d resources
		Microsoft::WRL::ComPtr<ID3D11Resource> m_DepthStencilTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_DepthStencilView;


	};
}
