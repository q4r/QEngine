#pragma once

#include <string>
#include <fstream>

#include "Surface.h"

struct Pos {
	float x, y, z;
};

struct Joint {
	std::string name;
	int parent;
	Pos pos;
	Pos orient;
};

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
	Pos pos;
};

struct Mesh{
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
	Joint* joints;
	Mesh* meshes;

	unsigned int version;
	unsigned int numJoints;
	unsigned int numMeshes;
	unsigned int numLoadedMesh;

	bool LoadJoints(std::ifstream& file, unsigned int count);
	bool LoadMesh(std::ifstream& file);
	bool LoadVerts(std::ifstream& file, unsigned int count, Mesh& mesh);
	bool LoadTris(std::ifstream& file, unsigned int count, Mesh& mesh);
	bool LoadWeights(std::ifstream& file, unsigned int count, Mesh& mesh);
public:
	MD5Loader();
	~MD5Loader(void);

	bool Init(const std::string& fileName);
	bool GetMesh(unsigned int index, Surface& surface);
};

