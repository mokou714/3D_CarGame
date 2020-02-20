#include "GameObjectRendererWithTex.h"

using namespace CarGame;

GameObjectRendererWithTex::GameObjectRendererWithTex(std::shared_ptr<GameObject> gameObject, Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dImmediateContext, std::shared_ptr<Camera> camera, const wchar_t* tex_file, bool isSkybox) 
	: GameObjectRenderer(gameObject, m_d3dDevice,m_d3dImmediateContext, camera), tex_file(tex_file), isSkybox(isSkybox)
{
}

void GameObjectRendererWithTex::LoadResources(){
	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	const wchar_t* VSFile = isSkybox? L"Shaders/SkyVertex.hlsl" : L"Shaders/TexVertexShader.hlsl";
	const wchar_t* PSFile = isSkybox? L"Shaders/SkyPixel.hlsl": L"Shaders/TexPixelShader.hlsl";

	// Compile vertex shader shader
	CheckIfFailed(CompileShader(VSFile, "main", "vs_5_0", Blob.ReleaseAndGetAddressOf()));
	//load vertex shader
	CheckIfFailed(m_d3dDevice->CreateVertexShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf()));

	//create vertex layout
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"Texcoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	CheckIfFailed(
		m_d3dDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), Blob->GetBufferPointer(), Blob->GetBufferSize(), &m_VertexLayout)
	);
	//Compile pixel shader shader
	CheckIfFailed(CompileShader(PSFile, "main", "ps_5_0", Blob.ReleaseAndGetAddressOf()));
	//load pixel shader
	CheckIfFailed(m_d3dDevice->CreatePixelShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf()));
	
	//Compile shadow vertex shader 
	CheckIfFailed(CompileShader(L"Shaders/ShadowVertexShader.hlsl", "main", "vs_5_0", Blob.ReleaseAndGetAddressOf()));
	CheckIfFailed(m_d3dDevice->CreateVertexShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, m_ShadowVertexShader.GetAddressOf()));

	//Compile shadow pixel shader
	CheckIfFailed(CompileShader(L"Shaders/ShadowPixelShader.hlsl", "main", "ps_5_0", Blob.ReleaseAndGetAddressOf()));
	CheckIfFailed(m_d3dDevice->CreatePixelShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, m_ShadowPixelShader.GetAddressOf()));

	Blob.Reset();

	//init VS cbuffer desc
	D3D11_BUFFER_DESC vcbd;
	ZeroMemory(&vcbd, sizeof(vcbd));
	vcbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vcbd.ByteWidth = sizeof(VSConstantBuffer);
	vcbd.Usage = D3D11_USAGE_DYNAMIC;
	vcbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//create VS cbuffer
	CheckIfFailed(m_d3dDevice->CreateBuffer(&vcbd, nullptr, m_ConstantBuffer[0].GetAddressOf()));

	//init PS cbuffer desc
	D3D11_BUFFER_DESC pcbd;
	ZeroMemory(&pcbd, sizeof(pcbd));
	pcbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pcbd.ByteWidth = sizeof(PSConstantBuffer); //must be a multiple of 16
	pcbd.Usage = D3D11_USAGE_DYNAMIC;
	pcbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//create PS cbuffer
	CheckIfFailed(m_d3dDevice->CreateBuffer(&pcbd, nullptr, m_ConstantBuffer[1].GetAddressOf()));

	//init vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = gameObject->getVerticesCount() * sizeof(myTexVertex);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	//init vertex data
	D3D11_SUBRESOURCE_DATA vinitdata;
	vinitdata.pSysMem = gameObject->getVertices()
;
	vinitdata.SysMemPitch = 0;
	vinitdata.SysMemSlicePitch = 0;
	//create vertex buffer
	CheckIfFailed(m_d3dDevice->CreateBuffer(&vbd, &vinitdata, &m_VertexBuffer));

	//init index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = gameObject->getIndicesCount() * sizeof(unsigned short);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	//init indices data
	D3D11_SUBRESOURCE_DATA iinitdata;
	iinitdata.pSysMem = gameObject->getIndices();
	iinitdata.SysMemPitch = 0;
	iinitdata.SysMemSlicePitch = 0;
	//create index buffer
	CheckIfFailed(m_d3dDevice->CreateBuffer(&ibd, &iinitdata, &m_IndexBuffer));

	//store index size for drawing
	m_IndexCount = gameObject->getIndicesCount();

	//init shadow cbuffer desc
	D3D11_BUFFER_DESC svcbd;
	ZeroMemory(&svcbd, sizeof(svcbd));
	svcbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	svcbd.ByteWidth = sizeof(OrthoConstantBuffer);
	svcbd.Usage = D3D11_USAGE_DYNAMIC;
	svcbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//create shadow cbuffer
	CheckIfFailed(m_d3dDevice->CreateBuffer(&svcbd, nullptr, m_ConstantBuffer[2].GetAddressOf()));

	//init texture
	CheckIfFailed(DirectX::CreateWICTextureFromFile(m_d3dDevice.Get(), tex_file, nullptr, m_Texture.GetAddressOf()));
	
	//init normal texture sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	CheckIfFailed(m_d3dDevice->CreateSamplerState(&sampDesc, m_WrapStyleSampler.GetAddressOf()));

	//init shadow texture sampler state
	//sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	//sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	//sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;


	//from MS doc
	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.BorderColor[0] = 1.0f;
	comparisonSamplerDesc.BorderColor[1] = 1.0f;
	comparisonSamplerDesc.BorderColor[2] = 1.0f;
	comparisonSamplerDesc.BorderColor[3] = 1.0f;
	comparisonSamplerDesc.MinLOD = 0.f;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	comparisonSamplerDesc.MipLODBias = 0.f;
	comparisonSamplerDesc.MaxAnisotropy = 0;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

	//comparisonSamplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;

	CheckIfFailed(m_d3dDevice->CreateSamplerState(&comparisonSamplerDesc, m_ClampStyleSampler.GetAddressOf()));


	//for texture rendering, render both front and back faces
	D3D11_RASTERIZER_DESC Ras_desc;
	ZeroMemory(&Ras_desc, sizeof(D3D11_RASTERIZER_DESC));
	Ras_desc.FillMode = D3D11_FILL_SOLID;
	Ras_desc.CullMode = D3D11_CULL_NONE; //no backculling
	CheckIfFailed(m_d3dDevice->CreateRasterizerState(&Ras_desc, &m_ResterizerState));

	D3D11_DEPTH_STENCIL_DESC DS_Desc;
	ZeroMemory(&DS_Desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DS_Desc.DepthEnable = true;
	DS_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DS_Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	CheckIfFailed(m_d3dDevice->CreateDepthStencilState(&DS_Desc, &m_DepthStencilState));

	D3D11_RASTERIZER_DESC shadowRenderStateDesc;
	ZeroMemory(&shadowRenderStateDesc, sizeof(D3D11_RASTERIZER_DESC));
	shadowRenderStateDesc.CullMode = D3D11_CULL_FRONT;
	shadowRenderStateDesc.FillMode = D3D11_FILL_SOLID;
	shadowRenderStateDesc.DepthClipEnable = true;
	CheckIfFailed(m_d3dDevice->CreateRasterizerState(&shadowRenderStateDesc, &m_ShadowRenderState));
}

bool GameObjectRendererWithTex::Render() {
	//update Vertex and Pixel cbuffer
	UpdateVertexShaderConstantBuffer();
	UpdatePixelShaderConstantBuffer();

	/******************************************
		bind resources to rendering pipeline
	******************************************/

	//bind topology and layout
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_d3dImmediateContext->IASetInputLayout(m_VertexLayout.Get());
	//bind shaders
	m_d3dImmediateContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	m_d3dImmediateContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	//bind sampler
	m_d3dImmediateContext->PSSetSamplers(0, 1, m_WrapStyleSampler.GetAddressOf());
	m_d3dImmediateContext->PSSetShaderResources(0, 1, m_Texture.GetAddressOf());
	//bind sampler to pixel shader
	m_d3dImmediateContext->PSSetSamplers(1, 1, m_ClampStyleSampler.GetAddressOf());
	//bind PS shader resource view outside
	//resterization & depthstencil
	m_d3dImmediateContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);
	m_d3dImmediateContext->RSSetState(m_ResterizerState.Get());
	//bind vertex buffer
	UINT stride = sizeof(myTexVertex);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	//bind index buffer, each index is one 16-bit unsigned integer (short).
	m_d3dImmediateContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	//bind vertex shader buffer
	m_d3dImmediateContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer[0].GetAddressOf());
	//bind pixel shader buffer
	m_d3dImmediateContext->PSSetConstantBuffers(1, 1, m_ConstantBuffer[1].GetAddressOf());
	//bind light view buffer
	m_d3dImmediateContext->VSSetConstantBuffers(2, 1, m_ConstantBuffer[2].GetAddressOf());
	//draw
	m_d3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);
	return true;
}

void GameObjectRendererWithTex::RenderDepth() {
	UpdateOrthoConstantBuffer();

	//bind topology and layout
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_d3dImmediateContext->IASetInputLayout(m_VertexLayout.Get());

	//bind shadow vertex&pixel shaders
	m_d3dImmediateContext->VSSetShader(m_ShadowVertexShader.Get(), nullptr, 0);
	m_d3dImmediateContext->PSSetShader(m_ShadowPixelShader.Get(), nullptr, 0);

	//bind vertex shader buffer(bind light view matrix)
	m_d3dImmediateContext->VSSetConstantBuffers(2, 1, m_ConstantBuffer[2].GetAddressOf());

	//bind rasterization state for depth rendering
	m_d3dImmediateContext->RSSetState(m_ShadowRenderState.Get());

	//bind vertex buffer
	UINT stride = sizeof(myTexVertex);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	//bind index buffer, each index is one 16-bit unsigned integer (short).
	m_d3dImmediateContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//draw
	m_d3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);

}




