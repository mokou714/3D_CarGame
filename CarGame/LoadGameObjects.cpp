#include "LoadGameObjects.h"

//hard code all game objects here

std::vector<std::shared_ptr<CarGame::GameObject>> CarGame::LoadGameObjects() {
	std::vector<std::shared_ptr<CarGame::GameObject>> vec;

	//Car
	static const VertexPosColor car_pos_color[] = {
		//part1 left
		{XMFLOAT3(-0.35f, 0.2f, 0.4f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(-0.35f, 0.0f, 0.7f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(-0.35f, 0.0f, -0.7f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(-0.35f, 0.5f, -0.4f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(-0.35f, 0.5f, 0.4f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		
		//part2 left
		{XMFLOAT3(-0.35f, -0.3f, 0.7f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(-0.35f, -0.3f, -0.7f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},

		//part1 right
		{XMFLOAT3(0.35f, 0.2f, 0.4f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(0.35f, 0.0f, 0.7f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(0.35f, 0.0f, -0.7f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(0.35f, 0.5f, -0.4f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(0.35f, 0.5f, 0.4), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},

		//part2 right
		{XMFLOAT3(0.3f, -0.3f, 0.7f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(0.3f, -0.3f, -0.7f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},

		//left front wheel
		//left back wheel
		//right front wheel
		//right back wheel
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
	myVertex* car_vertices = calculate_normal(car_pos_color, car_indices, ARRAYSIZE(car_indices));

	//cube
	static const VertexPosColor cube_pos_color[] = {
		{XMFLOAT3(0.5f,   0.5f,  0.5f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(0.5f,   0.5f, -0.5f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(0.5f,  -0.5f, -0.5f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(0.5f,  -0.5f,  0.5f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
		{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
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

	myVertex* cube_vertices = calculate_normal(cube_pos_color, cube_indices, ARRAYSIZE(cube_indices));

	//ground
	static const XMFLOAT4 groundColor(16 / 255.f, 111 / 255.f, 156 / 255.f, 1.0f);
	static const VertexPosColor ground_pos_color[] = {
		{XMFLOAT3(-10.0f, -1.f, 10.0f), groundColor},
		{XMFLOAT3(10.0f, -1.f,  10.0f), groundColor},
		{XMFLOAT3(-10.0f, -1.f, -10.0f), groundColor},
		{XMFLOAT3(10.0f,  -1.f,  -10.0f), groundColor}
	};
	static const unsigned short ground_indices[] = {
		0,1,2,
		2,1,3
	};
	myVertex* ground_vertices = calculate_normal(ground_pos_color, ground_indices, ARRAYSIZE(ground_indices));

	vec.emplace_back(
		std::shared_ptr<GameObject>(new GameObject("Car", car_vertices, sizeof(car_pos_color) / sizeof(VertexPosColor), car_indices, sizeof(car_indices) / sizeof(unsigned short)))
	);

	vec.emplace_back(
		std::shared_ptr<GameObject>(new GameObject("Ground", ground_vertices, sizeof(ground_pos_color) / sizeof(VertexPosColor), ground_indices, sizeof(ground_indices) / sizeof(unsigned short)))
	);

	vec.emplace_back(
		std::shared_ptr<GameObject>(new GameObject("cube", cube_vertices, sizeof(cube_pos_color) / sizeof(VertexPosColor), cube_indices, sizeof(cube_indices) / sizeof(unsigned short)))
	);

	//load outside models
	//WaveFrontReader<unsigned short> obj_reader;
	//obj_reader.Load(L"Models/table.obj");
	//unsigned int v_size = obj_reader.vertices.size();
	//static std::vector<mVertex> obj_vertices;
	//unsigned int i_size = obj_reader.indices.size();
	//static std::vector<unsigned short> indices = obj_reader.indices;
	//for (auto& vertex : obj_reader.vertices) {
	//	obj_vertices.push_back({ vertex.position, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) });
	//}
	//vec.emplace_back(
	//	std::shared_ptr<GameObject>(new GameObject("Tree", &obj_vertices[0], v_size, &indices[0], i_size))
	//);

	return vec;
}

//Smooth shading normals
static myVertex* CarGame::calculate_normal(const VertexPosColor* vertices, const unsigned short* indices, unsigned int i_size) {
	myVertex* myVertices = new myVertex[i_size];
	//init normals
	for (int i = 0; i < i_size;++i) {
		myVertices[i].normal = { 0.0f,0.0f,0.0f };
	}
	//calculate and add normals
	for (int i = 0; i < i_size; i += 3) {
		XMFLOAT3 pos1 = vertices[indices[i]].pos;
		XMFLOAT3 pos2 = vertices[indices[i + 1]].pos;
		XMFLOAT3 pos3 = vertices[indices[i + 2]].pos;

		XMVECTOR vec1 = XMVectorSet(pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z, 0.0f); //1->2
		XMVECTOR vec2 = XMVectorSet(pos3.x - pos1.x, pos3.y - pos1.y, pos3.z - pos1.z, 0.0f); //1->3

		XMFLOAT3 normal;
		XMStoreFloat3(&normal, XMVector3Normalize(XMVector3Cross(vec2, vec1)));

		for (int j = 0; j < 3; ++j) {
			myVertices[indices[i + j]].pos = vertices[indices[i + j]].pos;
			myVertices[indices[i + j]].color = vertices[indices[i + j]].color;
			myVertices[indices[i + j]].normal.x += normal.x;
			myVertices[indices[i + j]].normal.y += normal.y;
			myVertices[indices[i + j]].normal.z += normal.z;
		}
	}
	//average normals
	for (int i = 0; i < i_size; ++i) {
		XMVECTOR nor = XMVector3Normalize(XMLoadFloat3(&myVertices[i].normal));
		XMStoreFloat3(&myVertices[i].normal, nor);
	}
	return myVertices;
}
