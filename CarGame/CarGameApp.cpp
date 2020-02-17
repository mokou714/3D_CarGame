#include "CarGameApp.h"
#include "LoadGameObjects.h"

using namespace CarGame;



//public
CarGameApp::CarGameApp(HINSTANCE instance):d3dApp(instance){
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
	updateGameObjects();
	updateMouseControl();
	updateKeyboardControl();
	return;
}

//called per frame
void CarGameApp::RenderScene() {
	assert(m_d3dImmediateContext);
	assert(m_SwapChain);

	//clear and setup render target view and depth stencil view
	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
	m_d3dImmediateContext->ClearRenderTargetView(m_RenderTargetView.Get(), black);
	m_d3dImmediateContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	
	//Render the scene objects.
	if(renderObjects() )
		//Present scene if succeeded
		CheckIfFailed(m_SwapChain->Present(0, 0));
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
	cam->updateLookingAngle(m_pMouse->xPos * 0.0001f, m_pMouse->yPos * 0.0001f);
	cam->updateLookingDistance(m_pMouse->scrollWheelValue * 10);
	m_pMouse->scrollWheelValue /= 1.03f;

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
}

void CarGameApp::updateCar(Car* car) {
	if (m_pKeyboard->state.W) {
		//translate car in local space
		XMVECTOR world_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR local_forward = XMVector3Transform(world_forward, XMMatrixRotationY(car->getRotation().y));
		XMFLOAT3 forward_dir;
		XMStoreFloat3(&forward_dir, local_forward);
		car->Translate(forward_dir, MOVING_SPEED);
		cam->updateTranslation();
		//update wheel rotation
		car->updateWheels(forward);
		//interpolate wheel angle to 0 if not pressing A and D
		if (!m_pKeyboard->state.A && !m_pKeyboard->state.D)
			car->interpolateTurningAngle(0);
	}
	if (m_pKeyboard->state.S) {
		XMVECTOR world_backward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		XMVECTOR local_forward = XMVector3Transform(world_backward, XMMatrixRotationY(car->getRotation().y));
		XMFLOAT3 backward_dir;
		XMStoreFloat3(&backward_dir, local_forward);
		car->Translate(backward_dir, MOVING_SPEED);
		cam->updateTranslation();
		//update wheel rotation
		car->updateWheels(backward);
		//interpolate wheel angle to 0 if not pressing A and D
		if (!m_pKeyboard->state.A && !m_pKeyboard->state.D)
			car->interpolateTurningAngle(0);
	}
	if (m_pKeyboard->state.A) {
		//FirstPerson, only rotate wheels unless car is moving
		if (cam->mode == ThirdPerson) {
			//reverse A/D when moving backward
			if (m_pKeyboard->state.W)
				car->Rotate(XMFLOAT3(0.0f, -1.0f, 0.0f), TURNING_SPEED);
			else if (m_pKeyboard->state.S)
				car->Rotate(XMFLOAT3(0.0f, 1.0f, 0.0f), TURNING_SPEED);
			//update wheel angle
			car->updateWheels(leftward);
		}
		//ThirdPerson, keyboard not used

	}
	if (m_pKeyboard->state.D) {
		//FirstPerson, only rotate wheels unless car is moving
		if (cam->mode == ThirdPerson) {
			//reverse A/D when moving backward
			if (m_pKeyboard->state.W)
				car->Rotate(XMFLOAT3(0.0f, 1.0f, 0.0f), TURNING_SPEED);
			else if (m_pKeyboard->state.S)
				car->Rotate(XMFLOAT3(0.0f, -1.0f, 0.0f), TURNING_SPEED);
			//update wheel angle
			car->updateWheels(rightward);
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