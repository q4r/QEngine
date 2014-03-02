#pragma once

#include <string>
#include <fstream>

#include "Node.h"
#include "Mesh.h"
#include "Scene.h"

class MD5Loader{
private:
public:
	MD5Loader();
	~MD5Loader();

	Mesh* Init(Scene& scene, const std::string& path, const std::string meshName, Shader* shader);
};

