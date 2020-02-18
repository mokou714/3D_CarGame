#pragma once

#include "d3dApp.h"
#include "GameObject.h"
#include "Car.h"
#include "GameObjectRenderer.h"
#include "GameObjectRendererWithTex.h"
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
		std::vector<std::shared_ptr<GameObjectRenderer>> Renderers;
		bool renderObjects();
		std::shared_ptr<Camera> cam;
		bool isRenderingShadow;

		float dt;

		void initRenderers();

		//update Keyboard/Mouse
		void updateGameObjects();
		void updateMouseControl();
		void updateKeyboardControl();
		void updateCar(Car* car);
	
	};

}