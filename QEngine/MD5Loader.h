#pragma once

#include <string>
#include <fstream>

class MD5Loader{
private:
	unsigned int version;
	unsigned int numJoits;
	unsigned int numMeshes;

	bool LoadJoints(std::ifstream& file, unsigned int count);
public:
	MD5Loader(const std::string& fileName);
	~MD5Loader(void);
};

