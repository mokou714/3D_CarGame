#include "CarGameApp.h"
#include "LoadGameObjects.h"

using namespace CarGame;

#define TURNING_SPEED 0.005f
#define MOVING_SPEED 0.01f

//public
CarGameApp::CarGameApp(HINSTANCE instance):d3dApp(instance){
	//set up window app and d3d
	if (!Init())
		exit(0);

	//load game objects
	gameObjects = LoadGameObjects();

	//init camera, set first game object as target
	cam = std::shared_ptr<Camera>(new Camera(m_WindowWidth, m_WindowHeight, gameObjects[0]));
	cam->setPosition(0.0f, 1.5f, -1.0f);
	
	//init game object renderers
	for (auto obj_ptr:gameObjects) {
		if (obj_ptr->getName() == "Ground") {
			obj_ptr->setScale(10, 1, 10);
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/leaf_ground.jpg", false));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
		else if (obj_ptr->getName() == "Skybox") {
			obj_ptr->setScale(1000, 1000, 1000);
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/skybox.png", true));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
		else {
			auto renderer_ptr = std::shared_ptr<GameObjectRenderer>(new GameObjectRenderer(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam));
			renderer_ptr->init();
			Renderers.emplace_back(renderer_ptr);
		}
	}	

	//only init mouse
	m_pMouse->SetWindow(m_MainWindow);
	m_pMouse->SetMode(Mouse::MODE_ABSOLUTE);


}

CarGameApp::~CarGameApp(){}

//called per frame
void CarGameApp::UpdateScene(float dt) {
	updateGameObjects();
	updateMouseControl();
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
		renderer->Render();
	}
	return true;
}

void CarGameApp::updateGameObjects() {
	//get keyboard state
	Keyboard::State keyState = m_pKeyboard->GetState();
	Keyboard::State lastKeyState = m_KeyboardTracker.GetLastState();
	m_KeyboardTracker.Update(keyState);
	for (auto& obj : gameObjects) {
		if (obj->getName() == "Car") {
			if (keyState.IsKeyDown(Keyboard::W)) {
				//translate car in local space
				XMVECTOR world_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
				XMVECTOR local_forward = XMVector3Transform(world_forward, XMMatrixRotationY(obj->getRotation().y));
				XMFLOAT3 forward_dir;
				XMStoreFloat3(&forward_dir, local_forward);
				obj->Translate(forward_dir, MOVING_SPEED);
				cam->updateTranslation(forward_dir, MOVING_SPEED);
				((Car*)obj.get())->updateWheels(0);
			}
			if (keyState.IsKeyDown(Keyboard::S)) {
				XMVECTOR world_backward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
				XMVECTOR local_forward = XMVector3Transform(world_backward, XMMatrixRotationY(obj->getRotation().y));
				XMFLOAT3 backward_dir;
				XMStoreFloat3(&backward_dir, local_forward);
				obj->Translate(backward_dir, MOVING_SPEED);
				cam->updateTranslation(backward_dir, MOVING_SPEED);
				((Car*)obj.get())->updateWheels(0);
			}
			if (keyState.IsKeyDown(Keyboard::A)) {
				if (cam->mode == ThirdPerson) {
					obj->Rotate(XMFLOAT3(0.0f, -1.0f, 0.0f), TURNING_SPEED);
					((Car*)obj.get())->updateWheels(-1);
				}
				
			}
			if (keyState.IsKeyDown(Keyboard::D)) {
				if (cam->mode == ThirdPerson) {
					obj->Rotate(XMFLOAT3(0.0f, 1.0f, 0.0f), TURNING_SPEED);
					((Car*)obj.get())->updateWheels(1);
				}
				
			}
			if (keyState.IsKeyDown(Keyboard::F)) {
				cam->switchCamMode();
				Sleep(150);
			}
			if (keyState.IsKeyDown(Keyboard::R)){
				cam->reset3rdView();
				Sleep(150);
			}
		}
	}
}

void CarGameApp::updateMouseControl(){
	//get mouse state
	Mouse::State mouseState = m_pMouse->GetState();
	Mouse::State lastMouseState = m_MouseTracker.GetLastState();
	m_MouseTracker.Update(mouseState);
	//horizontal movement rotates camera around y axis
	cam->updateHorizontal((mouseState.x - lastMouseState.x) * 0.005f);
	//vertical movement rotates camera around x axis
	cam->updateVertical((mouseState.y - lastMouseState.y) * 0.005f);
	
}