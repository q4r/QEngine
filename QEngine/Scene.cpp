#include "Scene.h"
#include "Globals.h"

#include "ObjLoader.h"
#include "MD5Loader.h"

Scene::Scene(D3D& _d3d):
	d3d(_d3d),
	meshes(), 
	shaders(),
	textures(),
	camera(),
	root()
{

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

Mesh* Scene::CreateMesh(){
	Mesh* mesh = new Mesh(d3d.GetDevice(), d3d.GetContext());
	meshes.push_back(mesh);
	return mesh;
}

Mesh* Scene::CreateMeshFromOBJ(const std::string& fileName){
	Mesh* pMesh = new Mesh(d3d.GetDevice(), d3d.GetContext());

	ObjLoader objLoader;
	if ( objLoader.Init(fileName) ){
		return pMesh;
	}

	SAFEDELETE(pMesh);
	return NULL;
}

Mesh* Scene::CreateMeshFromMD5(const std::string& meshFileName, const std::string& animationFileName){
	Mesh* pMesh = new Mesh(d3d.GetDevice(), d3d.GetContext());

	//MD5Loader loader;
	//if (! loader.Init(meshFileName)){
	//	return false;
	//}

	//unsigned int surfaceCount = loader.GetSurfaceCount();

	//for (unsigned int surfaceIndex = 0; surfaceIndex < surfaceCount; surfaceIndex++){
	//	Surface* surface = new Surface(d3d.pDevice, d3d.pContext);
	//	std::string shader; //TEXTURE
	//	loader.GetSurface(surfaceIndex, *surface, shader);

	//	std::string textureFileName = "";
	//	if ( pathToMaterials.size() == 0 ){
	//		textureFileName = shader;
	//	}else{
	//		unsigned found = shader.find_last_of("/\\");
	//		//std::string path = mesh.shader.substr(0, found);
	//		textureFileName = pathToMaterials + shader.substr(found + 1);
	//	}
	//	Texture* texture = scene.GetTexture(textureFileName);

	//	surface->SetTexture(texture);

	//	surfaces.push_back(surface);
	//}

	return pMesh;
}


Shader* Scene::CreateShaderFromFile(const std::string& fileName, unsigned int additionalAttributes){
	Shader* shader = new Shader(d3d.GetDevice(), d3d.GetContext());
	if ( ! shader->Init(fileName, additionalAttributes) ){
		SAFEDELETE(shader);
	}else{
		shaders.push_back(shader);
	}
	return shader;
}

Texture* Scene::CreateTextureFromFile(const std::string& fileName){
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

Node* Scene::GetRoot(){
	return &root;
}

void Scene::Draw(){
	root.Draw(camera);
}