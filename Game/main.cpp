#include "Window.h"


Window* window;

Mesh* mesh;

Scene* scene;
Shader* shader;
Texture* texture;

Camera* camera;
float angle = 0.0;

void CreateSimpleMesh(Mesh* mesh, Shader* shader, Texture* texture){
	Surface::Vertices vertices;
	Surface::TexCoords texCoords;
	Surface::Normals normals;
	Surface::Indices indices;

	vertices.push_back(D3DXVECTOR3(-1.0f, -1.0f, 0.0f));
	vertices.push_back(D3DXVECTOR3(-1.0f,  1.0f, 0.0f));
	vertices.push_back(D3DXVECTOR3( 1.0f, -1.0f, 0.0f));
	vertices.push_back(D3DXVECTOR3( 1.0f,  1.0f, 0.0f));

	normals.push_back(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	normals.push_back(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	normals.push_back(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	normals.push_back(D3DXVECTOR3(0.0f, 0.0f, -1.0f));

	texCoords.push_back(D3DXVECTOR2(0.0f, 1.0f));
	texCoords.push_back(D3DXVECTOR2(0.0f, 0.0f));
	texCoords.push_back(D3DXVECTOR2(1.0f, 1.0f));
	texCoords.push_back(D3DXVECTOR2(1.0f, 0.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);

	mesh->AddSurface(vertices, texCoords, indices, shader, texture);
}

bool Init(){
	scene = window->GetNewScene();

	texture = scene->CreateTextureFromFile("../RES/BrickRound.jpg");
	
	//shader = scene->CreateShaderFromFile("../RES/Shader.fx", Shader::TEXCOORD0 | Shader::NORMAL);
	shader = scene->CreateShaderFromFile("../RES/Shader1.fx", Shader::TEXCOORD0);
	if ( ! shader ){
		return false;
	}

	mesh = scene->CreateMesh();
	CreateSimpleMesh(mesh, shader, texture);
	//mesh->LoadFromMD5("../RES/MD5/boblampcleanJPG.md5mesh", "../RES/MD5/");

	camera = scene->GetCamera();
	if ( ! camera ){
		return false;
	}

	camera->SetPosition(0.0f, 0.0f, -3.0f);

	//camera->SetPosition(0, 30, 80);
	//camera->LookAt(0, 30, 0);
	camera->SetAspect(8.0f/6.0f);
	//camera->LeftRight(0.5);

	Node* node = scene->GetRoot();
	node->SetMesh(mesh);
	node->SetPosition(0.1f, 0.0f, 0.0f);
	return true;
}

bool Logic(){
	//camera->SetPosition(cos(angle) * 80, 30, sin(angle) * 80);

	angle += 0.01f;
	//camera->LookAt(0, 30, 0);

	Node* node = scene->GetRoot();
	node->SetOrientation(D3DXVECTOR3(0, 0, -1), angle);

	return true;
}


void Draw(){
	scene->Draw();
}

bool Release(){
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int cmdShow){

	window = Window::GetWindow(hInstance, cmdShow, 800, 600);

	window->SetInitFunc(Init);
	window->SetLogicFunc(Logic);
	window->SetDrawFunc(Draw);
	window->SetReleaseFunc(Release);

	if (window){
		window->Run();
	}

	Window::Release();

	return 0;
}