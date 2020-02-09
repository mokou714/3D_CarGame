#include "CarGameApp.h"
#include "LoadGameObjects.h"

using namespace CarGame;

#define TURNING_SPEED 0.005f
#define MOVING_SPEED 0.01f
#define MOUSE_SENSITIVITY 5

//public
CarGameApp::CarGameApp(HINSTANCE instance):d3dApp(instance){
	//set up window app and d3d
	if (!Init())
		exit(0);

	//load game objects
	gameObjects = LoadGameObjects();

	//init camera, set first game object as target
	cam = std::shared_ptr<Camera>(new Camera(m_WindowWidth, m_WindowHeight, gameObjects[0]));
	cam->setPosition(0.0f, 1.2f, -1.0f);
	
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
		else if (obj_ptr->getName() == "Wheel") {
			auto renderer_ptr = std::shared_ptr<GameObjectRendererWithTex>(new GameObjectRendererWithTex(obj_ptr, m_d3dDevice, m_d3dImmediateContext, cam, L"Textures/tire.jpg", false));
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
		if(renderer->gameObject->getVisible())
			renderer->Render();
	}
	return true;
}

void CarGameApp::updateGameObjects() {
	for (auto& obj : gameObjects) {
		if (obj->getName() == "Car") {
			if (m_pKeyboard->state.W) {
				//translate car in local space
				XMVECTOR world_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
				XMVECTOR local_forward = XMVector3Transform(world_forward, XMMatrixRotationY(obj->getRotation().y));
				XMFLOAT3 forward_dir;
				XMStoreFloat3(&forward_dir, local_forward);
				obj->Translate(forward_dir, MOVING_SPEED);
				cam->updateTranslation();
				//update wheel rotation
				((Car*)obj.get())->updateWheels(forward);
				//interpolate wheel angle to 0 if not pressing A and D
				if (!m_pKeyboard->state.A && !m_pKeyboard->state.D)
					((Car*)obj.get())->interpolateTurningAngle(0);
			}
			if (m_pKeyboard->state.S) {
				XMVECTOR world_backward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
				XMVECTOR local_forward = XMVector3Transform(world_backward, XMMatrixRotationY(obj->getRotation().y));
				XMFLOAT3 backward_dir;
				XMStoreFloat3(&backward_dir, local_forward);
				obj->Translate(backward_dir, MOVING_SPEED);
				cam->updateTranslation();
				//update wheel rotation
				((Car*)obj.get())->updateWheels(backward);
				//interpolate wheel angle to 0 if not pressing A and D
				if (!m_pKeyboard->state.A && !m_pKeyboard->state.D)
					((Car*)obj.get())->interpolateTurningAngle(0);
			}
			if (m_pKeyboard->state.A) {
				//FirstPerson, only rotate wheels unless car is moving
				if (cam->mode == ThirdPerson) {
					//reverse A/D when moving backward
					if (m_pKeyboard->state.W)
						obj->Rotate(XMFLOAT3(0.0f, -1.0f, 0.0f), TURNING_SPEED);
					else if(m_pKeyboard->state.S)
						obj->Rotate(XMFLOAT3(0.0f, 1.0f, 0.0f), TURNING_SPEED);
					//update wheel angle
					((Car*)obj.get())->updateWheels(leftward);
				}
				//ThirdPerson, keyboard not used
				
			}
			if (m_pKeyboard->state.D) {
				//FirstPerson, only rotate wheels unless car is moving
				if (cam->mode == ThirdPerson) {
					//reverse A/D when moving backward
					if (m_pKeyboard->state.W)
						obj->Rotate(XMFLOAT3(0.0f, 1.0f, 0.0f), TURNING_SPEED);
					else if(m_pKeyboard->state.S)
						obj->Rotate(XMFLOAT3(0.0f, -1.0f, 0.0f), TURNING_SPEED);
					//update wheel angle
					((Car*)obj.get())->updateWheels(rightward);
				}
				//ThirdPerson, keyboard not used
				
				
			}
			if (m_pKeyboard->state.F) {
				cam->switchCamMode();
				obj->setVisible( cam->mode == FirstPerson ?false:true);
				Sleep(150);
			}
			if (m_pKeyboard->state.R){
				cam->reset3rdView();
				Sleep(150);
			}
		}
	}
}

void CarGameApp::updateMouseControl(){
	//horizontal movement rotates camera around y axis
	//vertical movement rotates camera around x axis
	cam->updateLookingAngle(m_pMouse->xPos * 0.0001f * MOUSE_SENSITIVITY, m_pMouse->yPos * 0.0001f * MOUSE_SENSITIVITY);
	cam->updateLookingDistance(m_pMouse->scrollWheelValue * 10);
	m_pMouse->scrollWheelValue /= 1.01;

}