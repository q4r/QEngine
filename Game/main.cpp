#include "Window.h"


Window* window;

Mesh* mesh;

Scene* scene;
Shader* shader;
Texture* texture;

Camera* camera;
float angle = 0.0;

void CreateSimpleMesh(Surface* surface){
	Surface::Vertices vertices;
	Surface::TexCoords texCoords;
	Surface::Normals normals;
	Surface::Indices indices;

	vertices.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	vertices.push_back(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	vertices.push_back(D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	vertices.push_back(D3DXVECTOR3(1.0f, 1.0f, 0.0f));

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
	indices.push_back(3);

	surface->Init(vertices, normals, texCoords, indices, NULL);
}

bool Init(){
	scene = window->GetNewScene();

	mesh = scene->GetMesh();
	mesh->LoadFromMD5("../RES/MD5/boblampcleanJPG.md5mesh", "../RES/MD5/");
	
	//shader = scene->GetShaderFromFile("../RES/Shader.fx", Shader::TEXCOORD0 | Shader::NORMAL);
	shader = scene->GetShaderFromFile("../RES/Shader1.fx", Shader::TEXCOORD0);
	if ( ! shader ){
		return false;
	}

	camera = scene->GetCamera();
	if ( ! camera ){
		return false;
	}

	camera->SetPosition(1, 1, -2);
	camera->LookAt(0, 0, 0);
	camera->SetAspect(8.0f/6.0f);
	//camera->LeftRight(0.5);

	return true;
}

bool Logic(){
	camera->SetPosition(cos(angle) * 80, 30, sin(angle) * 80);

	angle += 0.001f;
	camera->LookAt(0, 30, 0);

	return true;
}


void Draw(){
	if (shader){
		mesh->Draw(*shader, *camera);
	}
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