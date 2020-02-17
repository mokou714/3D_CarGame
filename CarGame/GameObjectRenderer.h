#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "d3dApp.h"
#include "Utilities.h"
#include "LightingStructures.h"


namespace CarGame {
	class GameObjectRenderer {
	public:
		GameObjectRenderer(std::shared_ptr<GameObject> gameObject, Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice,
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dImmediateContext, std::shared_ptr<Camera> camera);
		~GameObjectRenderer();
		std::shared_ptr<GameObject> gameObject;
		std::shared_ptr<Camera> m_Camera;
		virtual void init();
		virtual void LoadResources();
		virtual bool Render();
		void UpdateVertexShaderConstantBuffer();
		void UpdatePixelShaderConstantBuffer();
		void ReleaseResources();
		void RenderDepth();
		void UpdateOrthoConstantBuffer();
	protected:
		//app resources
		Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;
		//d3d resources
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_VertexLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer; //vertices
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;  //indeces
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_OrthoBuffer; //orthographic projection for shadow rendering
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer[3]; //0 vertex(WVP) buffer, 1 pixel(lighting) buffer, 2 ortho buffer
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_ResterizerState; //rasteriztion
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState; //depthstencil state for normal rendering
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_ClampStyleSampler; // //sample shadow depth texture
		VSConstantBuffer m_VSConstantBufferData;
		PSConstantBuffer m_PSConstantBufferData;
		OrthoConstantBuffer m_OrthoConstantBufferData;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_ShadowVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ShadowPixelShader;
		unsigned int m_IndexCount; //draw
		DirectionalLight m_DirLight; //light
		
	};
}
