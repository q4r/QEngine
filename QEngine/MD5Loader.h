#pragma once

#include <string>
#include <fstream>

#include "Surface.h"
#include "Node.h"

struct UV{
	float u, v;
};

struct Vert{
	unsigned int index;
	UV uv;
	unsigned int startWeight;
	unsigned int countWeight;
};

struct Tri{
	unsigned int index;
	unsigned int i0, i1, i2;
};

struct Weight{
	unsigned int index;
	unsigned int joint;
	float bias;
	D3DXVECTOR3 pos;
};

struct _Mesh{
	std::string shader;

	unsigned int numverts;
	Vert* verts;

	unsigned int numtris;
	Tri* tris;

	unsigned int numweights;
	Weight* weights;
};

class MD5Loader{
private:
	Node* joints;
	_Mesh* meshes;

	unsigned int version;
	unsigned int numJoints;
	unsigned int numMeshes;

	bool LoadJoints(std::ifstream& file, unsigned int count);
	bool LoadMesh(std::ifstream& file, unsigned int meshIndex);
	bool LoadVerts(std::ifstream& file, unsigned int count, _Mesh& mesh);
	bool LoadTris(std::ifstream& file, unsigned int count, _Mesh& mesh);
	bool LoadWeights(std::ifstream& file, unsigned int count, _Mesh& mesh);
public:
	MD5Loader();
	~MD5Loader(void);

	bool Init(const std::string& fileName);
	bool GetSurface(unsigned int index, Surface& surface, std::string& shader);
	unsigned int GetSurfaceCount();
};

