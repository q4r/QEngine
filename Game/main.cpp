#include "Window.h"


Window* window;

Surface* surface;
Surface* surfaceMD5_1;
Surface* surfaceMD5_2;
Surface* surfaceMD5_3;
Surface* surfaceMD5_4;
Surface* surfaceMD5_5;
Surface* surfaceMD5_6;

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

	MD5Loader loader;
	loader.Init("../RES/MD5/boblampcleanJPG.md5mesh");
	surfaceMD5_1 = scene->GetSurface();
	surfaceMD5_2 = scene->GetSurface();
	surfaceMD5_3 = scene->GetSurface();
	surfaceMD5_4 = scene->GetSurface();
	surfaceMD5_5 = scene->GetSurface();
	surfaceMD5_6 = scene->GetSurface();

	loader.GetMesh(0, *surfaceMD5_1, "../RES/MD5/");
	loader.GetMesh(1, *surfaceMD5_2, "../RES/MD5/");
	loader.GetMesh(2, *surfaceMD5_3, "../RES/MD5/");
	loader.GetMesh(3, *surfaceMD5_4, "../RES/MD5/");
	loader.GetMesh(4, *surfaceMD5_5, "../RES/MD5/");
	loader.GetMesh(5, *surfaceMD5_6, "../RES/MD5/");
	
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

	angle += 0.01f;
	camera->LookAt(0, 30, 0);

	return true;
}


void Draw(){
	if (shader){
		shader->Draw(surfaceMD5_1, camera);
		shader->Draw(surfaceMD5_2, camera);
		shader->Draw(surfaceMD5_3, camera);
		shader->Draw(surfaceMD5_4, camera);
		shader->Draw(surfaceMD5_5, camera);
		shader->Draw(surfaceMD5_6, camera);
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