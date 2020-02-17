#pragma once
#include <DirectXMath.h>
#include <memory>
#include "GameObject.h"

#define FIRST_PERSON_MOUSE_SENSITIVITY 10
#define THIRD_PERSON_MOUSE_SENSITIVITY 8

namespace CarGame {
	enum camera_mode{FirstPerson,ThirdPerson};

	class Camera{
	public:
		Camera(int WindowWidth, int WindowHeight, std::shared_ptr<GameObject> target);
		std::shared_ptr<GameObject> target;
		camera_mode mode;
		void setPosition(float x, float y, float z);
		void setLookingAt(float x, float y, float z);
		DirectX::XMVECTOR getPosition();
		DirectX::XMVECTOR getLookingAt();
		float getAspectRatio();
		void updateTranslation();
		void updateLookingAngle(float X_angleVelocity, float Y_angleVelocity);
		void updateLookingDistance(float scrollWheelValue);
		void switchCamMode();
		void reset3rdView();
	private:
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR upVector;
		DirectX::XMVECTOR lookingAt;
		int w_width;
		int w_height;
		DirectX::XMVECTOR dir_to_object;
		float distant_to_object;
		
	};
}