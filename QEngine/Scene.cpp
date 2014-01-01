#include "Scene.h"
#include "Globals.h"

Scene::Scene(D3D& _d3d):
	d3d(_d3d),
	surfaces(), 
	shaders(),
	textures(),
	camera(){
	LOG("Scene: constructor");
}


Scene::~Scene(void){
	for (Shaders::iterator i = shaders.begin(); i != shaders.end(); i++){
		SAFEDELETE(*i);
	}

	for (Textures::iterator i = textures.begin(); i != textures.end(); i++){
		SAFEDELETE(*i);
	}

	for (Surfaces::iterator i = surfaces.begin(); i != surfaces.end(); i++){
		SAFEDELETE(*i);
	}

	LOG("Scene: destructor");
}

//===============================================================================================

Surface* Scene::GetSurface(){
	Surface* surface = new Surface(d3d.GetDevice(), d3d.GetContext());
	surfaces.push_back(surface);
	return surface;
}

Surface* Scene::GetSurfaceFromObj(const std::string& fileName){
	Surface* surface = Surface::LoadFromObj(d3d.GetDevice(), d3d.GetContext(), fileName);
	if ( ! surface ){
		SAFEDELETE(surface);
	}else{
		surfaces.push_back(surface);
	}
	return surface;
}

Shader* Scene::GetShaderFromFile(const std::string& fileName, unsigned int additionalAttributes){
	Shader* shader = new Shader(d3d.GetDevice(), d3d.GetContext());
	if ( ! shader->Init(fileName, additionalAttributes) ){
		SAFEDELETE(shader);
	}else{
		shaders.push_back(shader);
	}
	return shader;
}

Texture* Scene::GetTexture(const std::string& fileName){
	Texture* texture = new Texture(d3d.GetDevice(), d3d.GetContext());
	if ( ! texture->Init(fileName)){
		SAFEDELETE(texture);
	}else{
		textures.push_back(texture);
	}
	return texture;
}

Camera* Scene::GetCamera(){
	return &camera;
}