#include "Window.h"


Window* window;

Surface* surface;
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

	surface->Init(vertices, normals, texCoords, indices);
}

bool Init(){
	MD5Loader loader("../RES/MD5/boblampclean.md5mesh");


	scene = window->GetNewScene();

	//surface = scene->GetSurface();
	//CreateSimpleMesh(surface);
	surface = scene->GetSurfaceFromObj("../RES/tetr.obj");
	if( ! surface ){
		return false;
	}

	shader = scene->GetShaderFromFile("../RES/Shader.fx", Shader::TEXCOORD0 | Shader::NORMAL);
	if ( ! shader ){
		return false;
	}

	texture = scene->GetTexture("../RES/BrickRound.jpg");
	if ( ! texture ){
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
	camera->SetPosition(cos(angle) * 3, 0, sin(angle) *3);
	angle += 0.01f;
	camera->LookAt(0, 0, 0);

	return true;
}


void Draw(){
	if (shader){
		shader->Draw(surface, camera, texture);
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