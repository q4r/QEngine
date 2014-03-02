#pragma once

#include <string>
#include "Surface.h"

class ObjLoader
{
	Surface::Vertices vertices;
	Surface::Normals  normals;
	Surface::TexCoords textureCoords;
	Surface::Indices  indices;

	struct Indices3{
		unsigned int vertex;
		unsigned int texcoord;
		unsigned int normal;
	};

	std::list<Indices3> indices3;

public:
	ObjLoader(void);
	~ObjLoader(void);

	bool Init(const std::string& fileName);
};

