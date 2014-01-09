#pragma once

#include <list>

#include "D3D.h"

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Scene
{
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

private:
	Scene(D3D& d3d);
	~Scene(void);

public:
	Mesh* GetMesh();

	Shader* GetShaderFromFile(const std::string& fileName, unsigned int additionalAttributes);

	Texture* GetTexture(const std::string& fileName);

	Camera* GetCamera();
};

