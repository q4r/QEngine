#pragma once

#include <list>

#include "D3D.h"
#include "Node.h"


class Scene{
	friend class Window;
private:
	typedef std::list<Mesh*> Meshes;
	typedef std::list<Shader*> Shaders;
	typedef std::list<Texture*> Textures;

	D3D& d3d;
	Meshes meshes;
	Shaders shaders;
	Textures textures;

	Camera camera;

	Node root;
	
private:
	Scene(D3D& d3d);
	~Scene(void);

public:
	Mesh* CreateMesh();
	Mesh* CreateMeshFromOBJ(const std::string& fileName);
	Mesh* CreateMeshFromMD5(const std::string& meshFileName, const std::string& animationFileName = "");

	Shader* CreateShaderFromFile(const std::string& fileName, unsigned int additionalAttributes);
	Texture* CreateTextureFromFile(const std::string& fileName);

	Camera* GetCamera();

	Node* GetRoot();

	void Draw();
};

