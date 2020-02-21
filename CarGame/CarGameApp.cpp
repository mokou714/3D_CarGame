#include "CarGameApp.h"
#include "LoadGameObjects.h"

using namespace CarGame;

//public
CarGameApp::CarGameApp(HINSTANCE instance):d3dApp(instance),dt(0.0f),isRenderingShadow(true){
	//set up window app and d3d
	if (!Init())
		exit(0);

	//load game objects
	gameObjects = LoadGameObjects();

	//initialize camera, set first game object as target
	cam = std::shared_ptr<Camera>(new Camera(m_WindowWidth, m_WindowHeight, gameObjects[0]));
	
	//initialize renderers for each game object
	initRenderers();
	
	
}

CarGameApp::~CarGameApp(){}

//called per frame
void CarGameApp::UpdateScene(float dt) {
	this->dt = dt;
	updateGameObjects();
	updateMouseControl();
	updateKeyboardControl();
	return;
}

//called per frame
void CarGameApp::RenderScene() {
	assert(m_d3dImmediateContext);
	assert(m_SwapChain);

	//clear render target view
	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
	m_d3dImmediateContext->ClearRenderTargetView(m_RenderTargetView.Get(), black);

	//clear shadow depth
	m_d3dImmediateContext->ClearDepthStencilView(m_Shadow_DepthStencilView.Get(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	//clear depth buffer
	m_d3dImmediateContext->ClearDepthStencilView(m_Normal_DepthStencilView.Get(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (isRenderingShadow) {
		//set shadow viewport
		m_d3dImmediateContext->RSSetViewports(1, &m_ShadowViewport);

		//set shadow depthstencil render target
		m_d3dImmediateContext->OMSetRenderTargets(0, nullptr, m_Shadow_DepthStencilView.Get());

		//render depth of all objects
		for (auto& renderer : Renderers) {
			//not render wheel, window shadow
			if (renderer->gameObject->getName() == "Wheel"
				|| renderer->gameObject->getName() == "Window"
				|| renderer->gameObject->getName() == "CarLight")
				continue;
			if (renderer->gameObject->getVisible())
				renderer->RenderDepth();
		}
	}

	//set normal depthstencil render target
	m_d3dImmediateContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_Normal_DepthStencilView.Get());
	//set normal screen viewport
	m_d3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);
	
	if(isRenderingShadow)
		//bind shadow texture after setting up normal depthstencil render target
		m_d3dImmediateContext->PSSetShaderResources(1, 1, m_ShadowSRV.GetAddressOf());
	
	//Render the scene objects.
	if(renderObjects() )
		//Present scene if succeeded
		CheckIfFailed(m_SwapChain->Present(0, 0));

	//unbind shadow texture after shadow mapping
	//ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	//m_d3dImmediateContext->PSSetShaderResources(1, 1, nullSRV);
	
}

//private
//render each object individually
bool CarGameApp::renderObjects() {
	for (auto& renderer : Renderers) {
		if(renderer->gameObject->getVisible())
			renderer->Render();
	}
	return true;
}

void CarGameApp::updateGameObjects() {
	for (auto& obj : gameObjects) {
		if (obj->getName() == "Car") {
			updateCar((Car*)obj.get());
		}
	}
}

void CarGameApp::updateMouseControl(){
	//horizontal movement rotates camera around y axis
	//vertical movement rotates camera around x axis
	cam->updateLookingAngle(-m_pMouse->xPos * 0.01f * dt, m_pMouse->yPos * 0.01f * dt);
	cam->updateLookingDistance(-m_pMouse->scrollWheelValue * dt);
	m_pMouse->scrollWheelValue /= (1 + 12*dt);

	m_pMouse->reset();

}

void CarGameApp::updateKeyboardControl() {
	if (m_pKeyboard->state.R) {
		cam->reset3rdView();
		Sleep(150);
	}
	if (m_pKeyboard->state.Esc) {
		ClipCursor(NULL);
		POINT pt;
		pt.x = m_WindowWidth / 2;
		pt.y = m_WindowHeight / 2;
		ClientToScreen(m_MainWindow, &pt);
		SetCursorPos(pt.x, pt.y);
		//ShowCursor uses internal counter, >=0 show, <0 hide
		while (ShowCursor(true) < 0)
			ShowCursor(true);
	}

	if (m_pKeyboard->state.G) {
		isRenderingShadow = !isRenderingShadow;
		Sleep(150);
	}
}

void CarGameApp::updateCar(Car* car) {
	if (m_pKeyboard->state.W) {
		//translate car in local space
		XMVECTOR world_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR local_forward = XMVector3Transform(world_forward, XMMatrixRotationY(car->getRotation().y));
		XMFLOAT3 forward_dir;
		XMStoreFloat3(&forward_dir, local_forward);
		car->Translate(forward_dir, MOVING_SPEED * dt);
		cam->updateTranslation();
		//update wheel rotation
		car->updateWheels(forward, dt);
		//interpolate wheel angle to 0 if not pressing A and D
		if (!m_pKeyboard->state.A && !m_pKeyboard->state.D)
			car->interpolateTurningAngle(0, dt);
	}
	if (m_pKeyboard->state.S) {
		XMVECTOR world_backward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		XMVECTOR local_forward = XMVector3Transform(world_backward, XMMatrixRotationY(car->getRotation().y));
		XMFLOAT3 backward_dir;
		XMStoreFloat3(&backward_dir, local_forward);
		car->Translate(backward_dir, MOVING_SPEED * dt);
		cam->updateTranslation();
		//update wheel rotation
		car->updateWheels(backward, dt);
		//interpolate wheel angle to 0 if not pressing A and D
		if (!m_pKeyboard->state.A && !m_pKeyboard->state.D)
			car->interpolateTurningAngle(0, dt);
	}
	if (m_pKeyboard->state.A) {
		//FirstPerson, only rotate wheels unless car is moving
		if (cam->mode == ThirdPerson) {
			//reverse A/D when moving backward
			if (m_pKeyboard->state.W)
				car->Rotate(XMFLOAT3(0.0f, -1.0f, 0.0f), TURNING_SPEED * dt);
			else if (m_pKeyboard->state.S)
				car->Rotate(XMFLOAT3(0.0f, 1.0f, 0.0f), TURNING_SPEED * dt);
			//update wheel angle
			car->updateWheels(leftward, dt);
		}
		//ThirdPerson, keyboard not used

	}
	if (m_pKeyboard->state.D) {
		//FirstPerson, only rotate wheels unless car is moving
		if (cam->mode == ThirdPerson) {
			//reverse A/D when moving backward
			if (m_pKeyboard->state.W)
				car->Rotate(XMFLOAT3(0.0f, 1.0f, 0.0f), TURNING_SPEED * dt);
			else if (m_pKeyboard->state.S)
				car->Rotate(XMFLOAT3(0.0f, -1.0f, 0.0f), TURNING_SPEED * dt);
			//update wheel angle
			car->updateWheels(rightward, dt);
		}
		//ThirdPerson, keyboard not used

	}
	if (m_pKeyboard->state.F) {
		cam->switchCamMode();
		car->setVisible(cam->mode == FirstPerson ? false : true);
		Sleep(150);
	}
}

void CarGameApp::initRenderers() {
	//init game object renderers
	for (auto obj_ptr : gameObjects) {
		std::string obj_name = obj_ptr->getName();
		if (obj_name == "Ground") {
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/leaf_ground.jpg", false));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
		else if (obj_name == "Skybox") {
			obj_ptr->setScale(1000, 1000, 1000);
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/skybox.png", true));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
		else if (obj_name == "Wheel") {
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/tire.jpg", false));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
		else if (obj_name == "Road") {
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/road.jpg", false));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
		else if (obj_name == "Tree") {
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/apple_tree.png", false));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
		else if (obj_name == "CarLight") {
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/car_light.jpg", false));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
		else if (obj_name == "FatTree") {
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/fat_tree.png", false));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
		else {
			auto renderer_ptr = std::shared_ptr<GameObjectRenderer>(new GameObjectRenderer(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
	}
}