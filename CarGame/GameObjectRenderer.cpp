#include "GameObjectRenderer.h"

using namespace CarGame;

GameObjectRenderer::GameObjectRenderer(std::shared_ptr<GameObject> gameObject, Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dImmediateContext, std::shared_ptr<Camera> cam) :
	gameObject(gameObject), m_d3dDevice(m_d3dDevice), m_d3dImmediateContext(m_d3dImmediateContext), m_Camera(cam),
	m_VSConstantBufferData(), m_PSConstantBufferData()
{
	
}

void GameObjectRenderer::init() {
	LoadResources();
	UpdateVertexShaderConstantBuffer();
	//init light and material
	m_DirLight.ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_DirLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_DirLight.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_DirLight.direction = XMFLOAT3(0.75f, -0.6f, -0.50f); //match the sun position on skybox


	m_PSConstantBufferData.material.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PSConstantBufferData.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PSConstantBufferData.material.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	

	m_PSConstantBufferData.dirLight = m_DirLight;
	XMStoreFloat3(&m_PSConstantBufferData.eyePos, m_Camera->getPosition());
	UpdatePixelShaderConstantBuffer();

}


GameObjectRenderer::~GameObjectRenderer() {
	ReleaseResources();
}

//load resources only oncec, when initialized
void GameObjectRenderer::LoadResources() {
	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	// Compile vertex shader
	CheckIfFailed(CompileShader(L"Shaders/ColorVertexShader.hlsl", "main", "vs_5_0", Blob.ReleaseAndGetAddressOf()));
	//load vertex shader
	CheckIfFailed(m_d3dDevice->CreateVertexShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf()));

	//create vertex layout
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	CheckIfFailed(
		m_d3dDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), Blob->GetBufferPointer(), Blob->GetBufferSize(), &m_VertexLayout)
	);
	//Compile pixel shader
	CheckIfFailed(CompileShader(L"Shaders/ColorPixelShader.hlsl", "main", "ps_5_0", Blob.ReleaseAndGetAddressOf()));
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
	vbd.ByteWidth = gameObject->getVerticesCount() * sizeof(myColorVertex);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	//init vertex data
	D3D11_SUBRESOURCE_DATA vinitdata;
	vinitdata.pSysMem = gameObject->getVertices();
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
	svcbd.ByteWidth = sizeof(VSConstantBuffer);
	svcbd.Usage = D3D11_USAGE_DYNAMIC;
	svcbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//create shadow cbuffer
	CheckIfFailed(m_d3dDevice->CreateBuffer(&svcbd, nullptr, m_ConstantBuffer[2].GetAddressOf()));

	//init rasterization state
	D3D11_RASTERIZER_DESC Ras_desc;
	ZeroMemory(&Ras_desc, sizeof(D3D11_RASTERIZER_DESC));
	Ras_desc.FillMode = D3D11_FILL_SOLID;
	Ras_desc.CullMode = D3D11_CULL_BACK; //backculling
	CheckIfFailed(m_d3dDevice->CreateRasterizerState(&Ras_desc, &m_ResterizerState));
	//init normal depth&stencil state
	D3D11_DEPTH_STENCIL_DESC DS_Desc1;
	ZeroMemory(&DS_Desc1, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DS_Desc1.DepthEnable = true;
	DS_Desc1.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DS_Desc1.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	CheckIfFailed(m_d3dDevice->CreateDepthStencilState(&DS_Desc1, &m_Normal_DepthStencilState));

	//init normal depth&stencil state
	D3D11_DEPTH_STENCIL_DESC DS_Desc2;
	ZeroMemory(&DS_Desc2, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DS_Desc2.DepthEnable = true;
	DS_Desc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DS_Desc2.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	CheckIfFailed(m_d3dDevice->CreateDepthStencilState(&DS_Desc2, &m_WriteDepth_DepthStencilState));
}

void GameObjectRenderer::UpdateVertexShaderConstantBuffer() {
	//update world, view, projection matrices for vertex buffer
	m_VSConstantBufferData.world = XMMatrixTranspose(gameObject->getTransformMatrix());
	m_VSConstantBufferData.view = XMMatrixTranspose(
		XMMatrixLookAtLH(
			m_Camera->getPosition(),
			m_Camera->getLookingAt(),
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		)
	);
	m_VSConstantBufferData.projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, m_Camera->getAspectRatio(), 1.0f, 1000.0f));
	//update inverse world view for normal calculation
	m_VSConstantBufferData.inv_world_view = XMMatrixInverse(nullptr,gameObject->getTransformMatrix());

	//use map() to update vertex buffer
	D3D11_MAPPED_SUBRESOURCE mappedData;
	CheckIfFailed(m_d3dImmediateContext->Map(m_ConstantBuffer[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBufferData, sizeof(VSConstantBuffer));
	m_d3dImmediateContext->Unmap(m_ConstantBuffer[0].Get(), 0);
	
}

void GameObjectRenderer::UpdatePixelShaderConstantBuffer() {
	//update eyePos
	XMStoreFloat3(&m_PSConstantBufferData.eyePos, m_Camera->getPosition());
	//use map() to update pixel buffer
	D3D11_MAPPED_SUBRESOURCE mappedData;
	CheckIfFailed(m_d3dImmediateContext->Map(m_ConstantBuffer[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBufferData, sizeof(PSConstantBuffer));
	m_d3dImmediateContext->Unmap(m_ConstantBuffer[1].Get(), 0);
	
}

//called per frame
bool GameObjectRenderer::Render() {
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
	//resterization & depthstencil
	m_d3dImmediateContext->OMSetDepthStencilState(m_Normal_DepthStencilState.Get(), 0);
	m_d3dImmediateContext->RSSetState(m_ResterizerState.Get());
	//bind vertex buffer
	UINT stride = sizeof(myColorVertex);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(),&stride, &offset);
	//bind index buffer, each index is one 16-bit unsigned integer (short).
	m_d3dImmediateContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	//bind vertex shader buffer
	m_d3dImmediateContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer[0].GetAddressOf());
	//bind pixel shader buffer
	m_d3dImmediateContext->PSSetConstantBuffers(1, 1, m_ConstantBuffer[1].GetAddressOf());
	//draw
	m_d3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);
	return true;
}

void GameObjectRenderer::UpdateOrthoConstantBuffer() {
	m_OrthoConstantBufferData.world = XMMatrixTranspose(gameObject->getTransformMatrix());
	m_OrthoConstantBufferData.view = XMMatrixTranspose(
		XMMatrixLookAtLH(
			-XMVectorSet(m_DirLight.direction.x, m_DirLight.direction.y, m_DirLight.direction.z, 1.0f),
			//m_Camera->getPosition(),
			XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		)
	);
	m_OrthoConstantBufferData.orthoProjMatrix = XMMatrixOrthographicOffCenterLH(0.0f, 600.0f, 0.0f, 400.0f, 0.1f, 100.0f);
	D3D11_MAPPED_SUBRESOURCE mappedData;
	CheckIfFailed(m_d3dImmediateContext->Map(m_ConstantBuffer[2].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(OrthoConstantBuffer), &m_OrthoConstantBufferData, sizeof(OrthoConstantBuffer));
	m_d3dImmediateContext->Unmap(m_ConstantBuffer[2].Get(), 0);
}

void GameObjectRenderer::RenderDepth() {
	UpdateOrthoConstantBuffer();

	//bind topology and layout
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_d3dImmediateContext->IASetInputLayout(m_VertexLayout.Get());

	//bind shadow vertex&pixel shaders
	m_d3dImmediateContext->VSSetShader(m_ShadowVertexShader.Get(), nullptr, 0);
	m_d3dImmediateContext->PSSetShader(m_ShadowPixelShader.Get(), nullptr, 0);

	//resterization & depthstencil
	m_d3dImmediateContext->OMSetDepthStencilState(m_WriteDepth_DepthStencilState.Get(), 0);
	m_d3dImmediateContext->RSSetState(m_ResterizerState.Get());

	//bind vertex buffer
	UINT stride = sizeof(myColorVertex);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	
	//bind index buffer, each index is one 16-bit unsigned integer (short).
	m_d3dImmediateContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//bind only shadow vertex shader constant buffer
	m_d3dImmediateContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer[2].GetAddressOf());

	//draw
	m_d3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);

}

void GameObjectRenderer::ReleaseResources() {
	m_VertexShader.Reset();
	m_VertexLayout.Reset();
	m_PixelShader.Reset();
	m_ConstantBuffer[0].Reset();
	m_ConstantBuffer[1].Reset();
	m_IndexBuffer.Reset();
	m_IndexCount = 0;
}

