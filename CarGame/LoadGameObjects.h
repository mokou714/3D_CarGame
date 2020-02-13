#pragma once
#include <vector>
#include "GameObject.h"
#include "Car.h"
#include "DirectXTex/DirectXTex.h"
#include "DirectXMesh/DirectXMesh.h"
#include "Utilities/WaveFrontReader.h"
#include <math.h>

namespace CarGame {
	//car
	static const XMFLOAT4 car_color = { 223 / 255.f, 227 / 255.f, 18 / 255.f, 1 };
	static const VertexPosColor car_pos_color[] = {
		//part1 left
		{XMFLOAT3(-0.35f, 0.2f, 0.4f), car_color},
		{XMFLOAT3(-0.35f, 0.0f, 0.7f), car_color},
		{XMFLOAT3(-0.35f, 0.0f, -0.7f), car_color},
		{XMFLOAT3(-0.35f, 0.5f, -0.4f), car_color},
		{XMFLOAT3(-0.35f, 0.5f, 0.4f), car_color},

		//part2 left
		{XMFLOAT3(-0.35f, -0.3f, 0.7f), car_color},
		{XMFLOAT3(-0.35f, -0.3f, -0.7f), car_color},

		//part1 right
		{XMFLOAT3(0.35f, 0.2f, 0.4f), car_color},
		{XMFLOAT3(0.35f, 0.0f, 0.7f), car_color},
		{XMFLOAT3(0.35f, 0.0f, -0.7f), car_color},
		{XMFLOAT3(0.35f, 0.5f, -0.4f), car_color},
		{XMFLOAT3(0.35f, 0.5f, 0.4), car_color},

		//part2 right
		{XMFLOAT3(0.35f, -0.3f, 0.7f), car_color},
		{XMFLOAT3(0.35f, -0.3f, -0.7f), car_color},
	};
	static const unsigned short car_indices[] = {
		//left
		0,2,1,
		0,3,2,
		0,4,3,
		1,2,5,
		5,2,6,
		//right counterclockwise
		9,7,8,
		10,7,9,
		11,7,10,
		9,8,12,
		9,12,13,

		//front
		8,1,12,
		12,1,5,
		7,0,8,
		8,0,1,
		11,4,7,
		7,4,0,

		//back counterclockwise
		10,9,3,
		3,9,2,
		13,2,9,
		2,13,6,

		//top
		10,3,11,
		11,3,4,

		//bottom counterclockwise
		6,13,12,
		12,5,6

	};
	//cube
	static const VertexPosColor cube_pos_color[] = {
		{XMFLOAT3(0.5f,   0.5f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT3(0.5f,   0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT3(0.5f,  -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT3(0.5f,  -0.5f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
	};
	static const unsigned short cube_indices[] = {
		//left
		0,3,1,
		1,3,2,
		//front
		4,0,5,
		5,0,1,
		//right
		7,4,6,
		6,4,5,
		//back
		3,7,2,
		2,7,6,
		//top
		0,4,3,
		3,4,7,
		//bottom
		5,1,6,
		6,1,2
	};
	//skybox
	static float edge_offset = 0.0005;
	static const VertexPosTex skybox_pos_tex[] = {
	{XMFLOAT3(-0.5f,   -0.5f,  -0.5f), XMFLOAT2(0.0f + edge_offset, 2 / 3.0f - edge_offset)}, //0,0.33
	{XMFLOAT3(-0.5f,    0.5f,  -0.5f), XMFLOAT2(0.0f + edge_offset, 1 / 3.0f + edge_offset)}, //0,0.66

	{XMFLOAT3(-0.5f,   -0.5f,  -0.5f), XMFLOAT2(0.25f + edge_offset, 1.0f)},//0.25,0
	{XMFLOAT3(-0.5f,   -0.5f,  0.5f), XMFLOAT2(0.25f + edge_offset, 2 / 3.0f - edge_offset)}, //0.25,0.33
	{XMFLOAT3(-0.5f,   0.5f,   0.5f), XMFLOAT2(0.25f + edge_offset, 1 / 3.0f + edge_offset)}, //0.25,0.66
	{XMFLOAT3(-0.5f,   0.5f,  -0.5f), XMFLOAT2(0.25f + edge_offset, 0.0f)}, //0.25, 1.0

	{XMFLOAT3(0.5f,    -0.5f,  -0.5f), XMFLOAT2(0.50f - edge_offset, 1.0f)},//0.5,0
	{XMFLOAT3(0.5f,    -0.5f,  0.5f), XMFLOAT2(0.50f - edge_offset, 2 / 3.0f - edge_offset)}, //0.5,0.33
	{XMFLOAT3(0.5f,    0.5f,   0.5f), XMFLOAT2(0.50f - edge_offset, 1 / 3.0f + edge_offset)},//0.5,0.66
	{XMFLOAT3(0.5f,    0.5f,  -0.5f), XMFLOAT2(0.50f - edge_offset, 0.0f)},//0.5,1.0

	{XMFLOAT3(0.5f,    -0.5f,  -0.5f), XMFLOAT2(0.75f, 2 / 3.0f - edge_offset)},//0.75,0.33
	{XMFLOAT3(0.5f,    0.5f,  -0.5f), XMFLOAT2(0.75f, 1 / 3.0f + edge_offset)},//0.75,0.66

	{XMFLOAT3(-0.5f,    -0.5f,  -0.5f), XMFLOAT2(1.0f - edge_offset, 2 / 3.0f - edge_offset)},//1.0,0.33
	{XMFLOAT3(-0.5f,    0.5f,  -0.5f), XMFLOAT2(1.0f - edge_offset, 1 / 3.0f + edge_offset)},//1.0,0.66
	};
	static const unsigned short skybox_indices[] = {
		//left
		4,1,0,
		4,0,3,
		//top
		4,5,9,
		4,9,8,
		//bottom
		2,3,7,
		2,7,6,
		//right
		7,10,11,
		7,11,8,
		//back
		10,12,13,
		10,13,11,
		//front
		7,8,4,
		7,4,3,

	};
	//ground
	static const XMFLOAT4 groundColor(16 / 255.f, 111 / 255.f, 156 / 255.f, 1.0f);
	static const VertexPosTex ground_pos_tex[] = {
		//left bottom is origin
		{XMFLOAT3(-10.0f, 0.f, 10.0f), XMFLOAT2(1.0f,0.0f)}, //left top
		{XMFLOAT3(10.0f, 0.f,  10.0f), XMFLOAT2(1.0f,1.0f)}, //right top
		{XMFLOAT3(-10.0f, 0.f, -10.0f), XMFLOAT2(0.0f,0.0f)}, //left bottom
		{XMFLOAT3(10.0f,  0.f,  -10.0f), XMFLOAT2(0.0f,1.0f)} //right bottom
	};
	static const unsigned short ground_indices[] = {
		0,1,2,
		2,1,3
	};

	//main loader
	std::vector<std::shared_ptr<GameObject>> LoadGameObjects();

	//static helpers
	//Smooth shading normals
	static myVertex* calculate_normal_from_pos_color(const VertexPosColor* vertices, const unsigned short* indices, unsigned int i_size);
	static myVertex* calculate_normal_from_pos_tex(const VertexPosTex* vertices, const unsigned short* indices, unsigned int i_size);
	//Calcuate wheel vertex positions and indices
	static VertexPosColor* generateWheelVertices(int side_count, float height, float width);
	static VertexPosTex* generateWheelVerticesWithTex(int side_count, float height, float width);
	static unsigned short* generateWheelIndices(int side_count);
	//Load Object from outside files
	static GameObject* LoadObjectFromFile(const wchar_t* file, const char* name);
	static Car* LoadCar();
	static GameObject* LoadWheel();
	static GameObject* LoadCube();
	static GameObject* LoadSkybox();
	static GameObject* LoadGround();
	static GameObject* LoadRoad();
}