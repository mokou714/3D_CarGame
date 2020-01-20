#pragma once

#include "d3dApp.h"
#include "GameObject.h"
#include "GameObjectRenderer.h"
#include "Camera.h"


namespace CarGame {

	class CarGameApp : public d3dApp {
	public:
		CarGameApp(HINSTANCE instance);
		~CarGameApp();
		void UpdateScene(float dt);
		void RenderScene();

	private:
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		std::vector<std::unique_ptr<GameObjectRenderer>> Renderers;
		bool renderObjects();
		std::shared_ptr<Camera> cam;

		//update Keyboard/Mouse
		void updateGameObjects();
		void updateMouseControl();
	
	};

}