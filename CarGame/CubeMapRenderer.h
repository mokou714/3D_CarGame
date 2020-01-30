#pragma once
#include "GameObjectRenderer.h"
#include "DDSTextureLoader/DDSTextureLoader.h"
#include "WICTextureLoader/WICTextureLoader.h"

namespace CarGame {
	class CubeMapRenderer : public GameObjectRenderer {
	public:
		CubeMapRenderer(std::shared_ptr<GameObject> gameObject, Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice,
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dImmediateContext, std::shared_ptr<Camera> camera, const wchar_t* tex_file);
		bool Render();
		void LoadResources();
	private:
		const wchar_t* tex_file;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture; //texture
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState; //sample
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_ResterizerState;
		Microsoft::WRL::ComPtr <ID3D11DepthStencilState> m_DepthStencilState;
	};
}
