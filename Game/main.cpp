#include "Window.h"

#include <MD5Loader.h>
#include <Globals.h>

Window* window;

Mesh* mesh;
Mesh* simpleMesh;

Scene* scene;
Shader* shader;
Texture* texture;

Node* node = NULL;
Node* node2 = NULL;
Node* node3 = NULL;

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

	texture = scene->CreateTextureFromFile("../RES/MD5/guard1_body.jpg");
	
	//shader = scene->CreateShaderFromFile("../RES/Shader.fx", Shader::TEXCOORD0 | Shader::NORMAL);
	shader = scene->CreateShaderFromFile("../RES/Shader1.fx", Shader::TEXCOORD0);
	if ( ! shader ){
		return false;
	}

	simpleMesh = scene->CreateMesh();
	CreateSimpleMesh(simpleMesh, shader, texture);
	//mesh->LoadFromMD5("../RES/MD5/boblampcleanJPG.md5mesh", "../RES/MD5/");

	camera = scene->GetCamera();
	if ( ! camera ){
		return false;
	}

	//camera->SetPosition(0.0f, 0.0f, -33.0f);

	camera->SetPosition(0, 0, -15);
	camera->LookAt(0, 0, 0);
	camera->SetAspect(8.0f/6.0f);
	//camera->LeftRight(0.5);

	MD5Loader loader;
	mesh = loader.Init(*scene, "../RES/MD5", "boblampclean", shader);
	if ( ! mesh ){
		return false;
	}

	node = scene->GetRoot()->CreateChild();
	//node->SetMesh(mesh);
	//===================
	node->SetMesh(simpleMesh);
	node->SetScale(0.3);
	//===================
	node->SetPosition(0.0f, 0, 0);

	//scene->GetRoot()->SetOrientation(D3DXVECTOR3(1, 0, 0), -90);

	node2 = node->CreateChild();
	node2->SetMesh(simpleMesh);
	node2->SetScale(.3);
	node2->SetPosition(3.0f, 0, 0);

	node3 = node2->CreateChild();
	node3->SetMesh(simpleMesh);
	node3->SetScale(.3);
	node3->SetPosition(.0f, 3.0f, 0);

	return true;
}

bool Logic(){
	//camera->SetPosition(cos(angle) * 80, 30, sin(angle) * 80);

	angle += 0.01f;
	//angle = 30.0f;
	//camera->LookAt(0, 30, 0);

	node->SetOrientation(D3DXVECTOR3(0, 0, 1), angle);
	node2->SetOrientation(D3DXVECTOR3(0, 0, 1), -2 * angle);

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