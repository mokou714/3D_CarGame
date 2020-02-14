#include "LoadGameObjects.h"

//hard code all game objects here

using namespace CarGame;

std::vector<std::shared_ptr<GameObject>> CarGame::LoadGameObjects() {
	//GameObject Container
	std::vector<std::shared_ptr<GameObject>> to_render;
	
	//car
	Car* car = LoadCar();
	to_render.emplace_back(std::shared_ptr<GameObject>(car));

	GameObject* leftFrontWheel = LoadWheel();
	GameObject* rightFrontWheel = LoadWheel();
	GameObject* leftRearWheel = LoadWheel();
	GameObject* rightRearWheel = LoadWheel();

	car->wheels.leftFrontWheel = leftFrontWheel;
	car->wheels.rightFrontWheel = rightFrontWheel;
	car->wheels.leftRearWheel = leftRearWheel;
	car->wheels.rightRearWheel = rightRearWheel;
	car->addChild(leftFrontWheel);
	car->addChild(rightFrontWheel);
	car->addChild(leftRearWheel);
	car->addChild(rightRearWheel);
	leftFrontWheel->parent = car;
	rightFrontWheel->parent = car;
	leftRearWheel->parent = car;
	rightRearWheel->parent = car;
	car->initWheelPositions();
	car->setPosition(0, 0.48, -10);
	
	//wheels
	to_render.emplace_back(std::shared_ptr<GameObject>(leftFrontWheel));
	to_render.emplace_back(std::shared_ptr<GameObject>(rightFrontWheel));
	to_render.emplace_back(std::shared_ptr<GameObject>(leftRearWheel));
	to_render.emplace_back(std::shared_ptr<GameObject>(rightRearWheel));
	
	//Cube
	//to_render.emplace_back(std::shared_ptr<GameObject>(LoadCube()));

	//car windows
	GameObject* leftFrontWindow = LoadWindow();
	GameObject* leftRearWindow = LoadWindow();
	GameObject* rightFrontWindow = LoadWindow();
	GameObject* rightRearWindow = LoadWindow();
	GameObject* frontWindow = LoadWindow();
	leftFrontWindow->setPosition(-0.3501f, 0.16f, 0.15f);
	leftRearWindow->setPosition(-0.3501f, 0.16f, -0.233f);
	rightFrontWindow->setPosition(0.3501f, 0.16f, 0.15f);
	rightRearWindow->setPosition(0.3501f, 0.16f, -0.233f);
	frontWindow->setPosition(0, 0.15f, 0.41f);
	frontWindow->setRotation(0, PI/2, 0);
	frontWindow->setScale(1, 1, 1.8);
	car->addChild(leftFrontWindow);
	car->addChild(leftRearWindow);
	car->addChild(rightFrontWindow);
	car->addChild(rightRearWindow);
	car->addChild(frontWindow);
	leftFrontWindow->parent = car;
	leftRearWindow->parent = car;
	rightFrontWindow->parent = car;
	rightRearWindow->parent = car;
	frontWindow->parent = car;
	to_render.emplace_back(std::shared_ptr<GameObject>(leftFrontWindow));
	to_render.emplace_back(std::shared_ptr<GameObject>(leftRearWindow));
	to_render.emplace_back(std::shared_ptr<GameObject>(rightFrontWindow));
	to_render.emplace_back(std::shared_ptr<GameObject>(rightRearWindow));
	to_render.emplace_back(std::shared_ptr<GameObject>(frontWindow));

	//car lights
	GameObject* light1 = LoadCarLight();
	GameObject* light2 = LoadCarLight();
	light1->setName("CarLight");
	light2->setName("CarLight");
	car->addChild(light1);
	car->addChild(light2);
	light1->parent = car;
	light2->parent = car;
	light1->setPosition(0.18, -0.18, 0.735);
	light1->setRotation(0, PI / 2, 0);
	light1->setScale(0.45, 0.45, 0.45);
	light2->setPosition(-0.18, -0.18, 0.735);
	light2->setRotation(0, PI / 2, 0);
	light2->setScale(0.45, 0.45, 0.45);
	to_render.emplace_back(std::shared_ptr<GameObject>(light1));
	to_render.emplace_back(std::shared_ptr<GameObject>(light2));

	//Skybox
	to_render.emplace_back(std::shared_ptr<GameObject>(LoadSkybox()));

	
	//Ground
	GameObject* ground = LoadGround();
	ground->setScale(3, 1, 3);
	to_render.emplace_back(std::shared_ptr<GameObject>(ground));
	
	//roads
	GameObject* road1 = LoadRoad();
	GameObject* road2 = LoadRoad();
	GameObject* road3 = LoadRoad();
	GameObject* road4 = LoadRoad();
	road1->setScale(0.1, 1, 0.6);
	road2->setScale(0.1, 1, 0.45);
	road3->setScale(0.1, 1, 0.45);
	road4->setScale(0.1, 1, 0.6);

	road1->setPosition(-5.5, 0.001, 0);
	road4->setPosition( 5.5, 0.001, 0);

	road2->setRotation(0, PI / 2, 0);
	road2->setPosition(0, 0.001, -5);
	road3->setRotation(0, PI / 2, 0);
	road3->setPosition(0, 0.001, 5);

	ground->addChild(road1);
	ground->addChild(road2);
	ground->addChild(road3);
	ground->addChild(road4);
	road1->parent = road2->parent = road3->parent = road4->parent = ground;
	to_render.emplace_back(std::shared_ptr<GameObject>(road1));
	to_render.emplace_back(std::shared_ptr<GameObject>(road2));
	to_render.emplace_back(std::shared_ptr<GameObject>(road3));
	to_render.emplace_back(std::shared_ptr<GameObject>(road4));

	//load objects from files
	GameObject* tree = LoadObjectFromFile(L"Models/fat_tree.obj", "FatTree");
	tree->setScale(0.3, 0.3, 0.3);
	to_render.push_back(std::shared_ptr<GameObject>(tree));
	GameObject* tree1 = LoadObjectFromFile(L"Models/apple_tree_01.obj", "Tree");
	tree1->setPosition(-9, 0, 8);
	tree1->setScale(0.5, 0.5, 0.5);
	to_render.push_back(std::shared_ptr<GameObject>(tree1));
	GameObject* tree2 = LoadObjectFromFile(L"Models/apple_tree_02.obj", "Tree");
	tree2->setPosition(10, 0, 11);
	tree2->setScale(0.5, 0.5, 0.5);
	to_render.push_back(std::shared_ptr<GameObject>(tree2));
	GameObject* tree3 = LoadObjectFromFile(L"Models/apple_tree_03.obj", "Tree");
	tree3->setPosition(9, 0, -10);
	tree3->setScale(0.5, 0.5, 0.5);
	to_render.push_back(std::shared_ptr<GameObject>(tree3));
	GameObject* tree4 = LoadObjectFromFile(L"Models/apple_tree_04.obj", "Tree");
	tree4->setPosition(-9,0, -10);
	tree4->setScale(0.5, 0.5, 0.5);
	to_render.push_back(std::shared_ptr<GameObject>(tree4));
	
	return to_render;
}

//Static Helpers
//Smooth shading normals
static myVertex* CarGame::calculate_normal_from_pos_color(const VertexPosColor* vertices, const unsigned short* indices, unsigned int i_size) {
	myColorVertex* myVertices = new myColorVertex[i_size];
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
static myVertex* CarGame::calculate_normal_from_pos_tex(const VertexPosTex* vertices, const unsigned short* indices, unsigned int i_size) {
	myTexVertex* myVertices = new myTexVertex[i_size];
	//init normals
	for (int i = 0; i < i_size; ++i) {
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
			myVertices[indices[i + j]].texcoord = vertices[indices[i + j]].texcoord;
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

//Calculate wheel vertex positions and indices
static VertexPosColor* CarGame::generateWheelVertices(int side_count, float height, float width) {
	VertexPosColor* result = new VertexPosColor[side_count*2+2];
	float rad = 2 * PI / side_count;
	//two halves
	for (int i = 0; i < side_count; ++i) {
		result[i] = {XMFLOAT3(width/2, std::sin(rad*i)*height / 2, std::cos(rad*i)*height/2), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
		result[i+side_count+1] = {XMFLOAT3(-width/2, std::sin(rad*i)*height / 2, std::cos(rad*i)*height / 2), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	}
	//center
	result[side_count] = { XMFLOAT3(width/2.0,0.0f,0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	result[side_count*2+1] = { XMFLOAT3(-width/2.0,0.0f,0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	return result;
}
static VertexPosTex* CarGame::generateWheelVerticesWithTex(int side_count, float height, float width) {
	VertexPosTex* result = new VertexPosTex[side_count * 2 + 2];
	float rad = 2 * PI / side_count;
	//two halves
	for (int i = 0; i < side_count; ++i) {
		result[i] = { XMFLOAT3(width / 2, std::sin(rad*i)*height / 2, std::cos(rad*i)*height / 2), XMFLOAT2(std::sin(rad*i)/2.5+0.5f,std::cos(rad*i)/2.5+0.5f) };
		result[i + side_count + 1] = { XMFLOAT3(-width / 2, std::sin(rad*i)*height / 2, std::cos(rad*i)*height / 2), XMFLOAT2(std::sin(rad*i)/2.5 + 0.5f,std::cos(rad*i)/2.5+0.5f) };
	}
	//center
	result[side_count] = { XMFLOAT3(width / 2.0,0.0f,0.0f), XMFLOAT2(0.5f,0.55f) };
	result[side_count * 2 + 1] = { XMFLOAT3(-width / 2.0,0.0f,0.0f), XMFLOAT2(0.5f,0.55f) };
	return result;
}
static unsigned short* CarGame::generateWheelIndices(int side_count) {
	unsigned short* result = new unsigned short[side_count*3*4];
	for (int i = 0; i < side_count; ++i) {
		//1st half
		result[i*3 + 0] = i; //0
		result[i*3 + 1] = (i + 1) % side_count; //1
		result[i*3 + 2] = side_count; //8
		//2nd half
		result[(i + side_count) * 3 + 0] = i + side_count + 1; //9
		result[(i + side_count) * 3 + 1] = (i + 1) % side_count + side_count + 1; //10
		result[(i + side_count) * 3 + 2] = side_count*2+1; //17
		//mid
		result[ (side_count*2) * 3 + 0 + i * 6] = i; //0
		result[ (side_count*2) * 3 + 1 + i * 6] = i + side_count + 1; //9
		result[ (side_count*2) * 3 + 2 + i * 6] = (i + 1) % side_count + side_count + 1; //10
		result[ (side_count*2) * 3 + 3 + i * 6] = i; //0
		result[ (side_count*2) * 3 + 4 + i * 6] = (i + 1 ) % side_count + side_count + 1; //10
		result[ (side_count*2) * 3 + 5 + i * 6] = (i + 1) % side_count; //1
	}
	return result;
}
//Calculate circle vertex position for light
static VertexPosTex* CarGame::generateCircle(int side_count, float height, float width) {
	VertexPosTex* result = new VertexPosTex[side_count + 1];
	float rad = 2 * PI / side_count;
	for (int i = 0; i < side_count; ++i) {
		result[i] = { XMFLOAT3(width / 2, std::sin(rad*i)*height / 2, std::cos(rad*i)*height / 2), XMFLOAT2(std::sin(rad*i) / 2.2 + 0.5f,std::cos(rad*i) / 2.2 + 0.5f) };
	}
	//center
	result[side_count] = { XMFLOAT3(width / 2.0,0.0f,0.0f),XMFLOAT2(0.5f,0.5f) };
	return result;
}
static unsigned short* CarGame::generateCircleIndices(int side_count) {
	unsigned short* result = new unsigned short[side_count * 3];
	for (int i = 0; i < side_count; ++i) {
		result[i * 3 + 0] = i;
		result[i * 3 + 1] = (i+1)%side_count;
		result[i * 3 + 2] = side_count;
	}
	return result;
}


//Load Object from outside files
static GameObject* CarGame::LoadObjectFromFile(const wchar_t* file, const char* name) {
	WaveFrontReader<unsigned short> obj_reader;
	obj_reader.Load(file);

	//vertices
	unsigned int v_size = obj_reader.vertices.size();
	myTexVertex* obj_vertices = new myTexVertex[v_size];

	//indices
	unsigned int i_size = obj_reader.indices.size();
	unsigned short* obj_indices = new unsigned short[i_size];

	//copy vertices
	for (int i = 0; i < v_size; ++i) {
		obj_vertices[i].pos = obj_reader.vertices[i].position;
		obj_vertices[i].normal = obj_reader.vertices[i].normal;
		obj_vertices[i].texcoord = obj_reader.vertices[i].textureCoordinate;
	}
	//copy indices
	for (int i = 0; i < i_size; ++i) {
		obj_indices[i] = obj_reader.indices[i];
	}

	return new GameObject(name, obj_vertices, v_size, obj_indices, i_size);

}

static Car* CarGame::LoadCar() {
	//Car
	XMFLOAT4 car_color = { 252.0 / 255.0f, 225.0 / 255.f, 20.0 / 255.f, 1.0f };

	myVertex* car_vertices = CarGame::calculate_normal_from_pos_color(car_pos_color, car_indices, ARRAYSIZE(car_indices));
	Car* car = new Car("Car", car_vertices, sizeof(car_pos_color) / sizeof(VertexPosColor), car_indices, sizeof(car_indices) / sizeof(unsigned short));
	return car;
}

static GameObject* CarGame::LoadWheel() {
	//wheel
	int wheel_side_count = 16;
	float wheel_height = 0.3;
	float wheel_width = 0.1f;
	static const VertexPosTex* wheel_pos_color = CarGame::generateWheelVerticesWithTex(wheel_side_count, wheel_height, wheel_width);
	static const unsigned short* wheel_indices = CarGame::generateWheelIndices(wheel_side_count);
	myVertex* wheel_vertices = CarGame::calculate_normal_from_pos_tex(wheel_pos_color, wheel_indices, wheel_side_count * 3 * 4);

	GameObject* wheel = new GameObject("Wheel", wheel_vertices, wheel_side_count * 2 + 2, wheel_indices, wheel_side_count * 3 * 4);
	return wheel;
}

static GameObject* CarGame::LoadCube() {
	myVertex* cube_vertices = CarGame::calculate_normal_from_pos_color(cube_pos_color, cube_indices, ARRAYSIZE(cube_indices));
	return new GameObject("Cube", cube_vertices, sizeof(cube_pos_color) / sizeof(VertexPosColor), cube_indices, sizeof(cube_indices) / sizeof(unsigned short));
}

static GameObject* CarGame::LoadSkybox() {
	myVertex* skybox_vertices = CarGame::calculate_normal_from_pos_tex(skybox_pos_tex, skybox_indices, ARRAYSIZE(skybox_indices));
	return new GameObject("Skybox", skybox_vertices, sizeof(skybox_pos_tex) / sizeof(VertexPosTex), skybox_indices, sizeof(skybox_indices) / sizeof(unsigned short));
}

static GameObject* CarGame::LoadGround() {
	myVertex* ground_vertices = CarGame::calculate_normal_from_pos_tex(ground_pos_tex, ground_indices, ARRAYSIZE(ground_indices));
	return new GameObject("Ground", ground_vertices, sizeof(ground_pos_tex) / sizeof(VertexPosTex), ground_indices, sizeof(ground_indices) / sizeof(unsigned short));
}

static GameObject* CarGame::LoadRoad(){
	myVertex* road_vertices = CarGame::calculate_normal_from_pos_tex(ground_pos_tex, ground_indices, ARRAYSIZE(ground_indices));
	return new GameObject("Road", road_vertices, sizeof(ground_pos_tex) / sizeof(VertexPosTex), ground_indices, sizeof(ground_indices) / sizeof(unsigned short));
}

static GameObject* CarGame::LoadWindow() {
	myVertex* window_vertices = CarGame::calculate_normal_from_pos_color(window_pos_color, window_indices, ARRAYSIZE(window_indices));
	return new GameObject("Window", window_vertices, sizeof(window_pos_color) / sizeof(VertexPosColor), window_indices, sizeof(window_indices) / sizeof(unsigned short));
}

static GameObject* CarGame::LoadCarLight() {
	int side_count = 16;
	float height = 0.3f;
	float width = 0.1f;
	VertexPosTex* light_vertices = generateCircle(side_count,height,width);
	unsigned short* light_indices = generateCircleIndices(side_count);
	//myVertex* vertices = calculate_normal_from_pos_color(light_vertices, light_indices, side_count * 3);
	myVertex* vertices = calculate_normal_from_pos_tex(light_vertices, light_indices, side_count * 3);
	return new GameObject("CarLight", vertices, side_count + 1, light_indices, side_count * 3);
}