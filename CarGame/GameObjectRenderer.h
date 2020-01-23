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
	protected:
		//app resources
		Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;
		//d3d resources
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_VertexLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer; //vertices
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;  //indeces
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer[2]; //0 vertex(WVP) buffer, 1 pixel(lighting) buffer
		VSConstantBuffer m_VSConstantBufferData;
		PSConstantBuffer m_PSConstantBufferData;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		unsigned int m_IndexCount; //draw
		DirectionalLight m_DirLight; //light
		
	};
}
