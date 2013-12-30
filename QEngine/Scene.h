#pragma once

#include <list>

#include "D3D.h"

#include "Surface.h"
#include "Shader.h"
#include "Texture.h"

class Scene
{
	friend class Window;
private:
	typedef std::list<Surface*> Surfaces;
	typedef std::list<Shader*> Shaders;
	typedef std::list<Texture*> Textures;

	D3D& d3d;
	Surfaces surfaces;
	Shaders shaders;
	Textures textures;

	Camera camera;

private:
	Scene(D3D& d3d);
	~Scene(void);

public:
	Surface* GetSurface();
	Surface* GetSurfaceFromObj(const std::string& fileName);

	Shader* GetShaderFromFile(const std::string& fileName, unsigned int additionalAttributes);

	Texture* GetTexture(const std::string& fileName);

	Camera* GetCamera();
};

