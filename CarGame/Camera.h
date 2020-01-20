#pragma once
#include <DirectXMath.h>
#include <memory>
#include "GameObject.h"

namespace CarGame {
	enum camera_mode{FirstPerson,ThirdPerson};

	class Camera {
	public:
		Camera(int WindowWidth, int WindowHeight, std::shared_ptr<GameObject> target);
		std::shared_ptr<GameObject> target;
		camera_mode mode;
		void setPosition(float x, float y, float z);
		void setLookingAt(float x, float y, float z);
		DirectX::XMVECTOR getPosition();
		DirectX::XMVECTOR getLookingAt();
		float getAspectRatio();
		void updateTranslation(XMFLOAT3 dir, float speed);
		void updateHorizontal(float angleVelocity);
		void updateVertical(float angleVelocity);
		void switchCamMode();
		void reset3rdView();
	private:
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR upVector;
		DirectX::XMVECTOR lookingAt;
		int w_width;
		int w_height;
		DirectX::XMVECTOR default_3rd_pos_offset;
		
	};
}