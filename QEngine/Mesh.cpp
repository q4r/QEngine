#include "Mesh.h"

#include "MD5Loader.h";

Mesh::Mesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, Scene& _scene) :
	pDevice(_pDevice), 
	pContext(_pContext), 
	scene(_scene),
	surfaces()
{
}

Mesh::~Mesh(){
}

bool Mesh::LoadFromMD5(const std::string& fileName, const std::string& pathToMaterials){
	MD5Loader loader;
	if (! loader.Init(fileName)){
		return false;
	}

	unsigned int surfaceCount = loader.GetSurfaceCount();

	for (unsigned int surfaceIndex = 0; surfaceIndex < surfaceCount; surfaceIndex++){
		Surface* surface = new Surface(pDevice, pContext);
		std::string shader; //TEXTURE
		loader.GetSurface(surfaceIndex, *surface, shader);

		std::string textureFileName = "";
		if ( pathToMaterials.size() == 0 ){
			textureFileName = shader;
		}else{
			unsigned found = shader.find_last_of("/\\");
			//std::string path = mesh.shader.substr(0, found);
			textureFileName = pathToMaterials + shader.substr(found + 1);
		}
		Texture* texture = scene.GetTexture(textureFileName);

		surface->SetTexture(texture);

		surfaces.push_back(surface);
	}

	return true;
}

void Mesh::Draw(Shader& shader, Camera& camera){
	for (Surfaces::iterator surface = surfaces.begin(); surface != surfaces.end(); surface++){
		shader.Draw(*surface, &camera);
	}	
}