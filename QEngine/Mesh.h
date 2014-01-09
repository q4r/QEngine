#pragma once

#include "Surface.h"
#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include <list>

class Mesh{
	friend class Scene;
private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	Scene& scene;

	typedef std::list<Surface*> Surfaces;
	Surfaces surfaces;

	Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Scene& scene);
	~Mesh();

public:
	//bool AddSurface(Surface* surface);
	bool LoadFromMD5(const std::string& fileName, const std::string& pathToMaterials);

	void Draw(Shader& shader, Camera& camera);
};

