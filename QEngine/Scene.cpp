#include "Scene.h"
#include "Globals.h"

Scene::Scene(D3D& _d3d):
	d3d(_d3d),
	meshes(), 
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

	for (Meshes::iterator i = meshes.begin(); i != meshes.end(); i++){
		SAFEDELETE(*i);
	}

	LOG("Scene: destructor");
}

//===============================================================================================

Mesh* Scene::GetMesh(){
	Mesh* mesh = new Mesh(d3d.GetDevice(), d3d.GetContext(), *this);
	meshes.push_back(mesh);
	return mesh;
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