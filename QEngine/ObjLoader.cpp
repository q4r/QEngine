#include "ObjLoader.h"

#include <fstream>
#include <sstream>
#include <list>

#include "Globals.h"

ObjLoader::ObjLoader(void)
{
}


ObjLoader::~ObjLoader(void)
{
}

bool ObjLoader::Init(const std::string& fileName){
	std::ifstream file;

	file.open(fileName.c_str());

	if (file.is_open()){
		LOG_("Surface: Loading .OBJ...");
		std::string line;
		while ( ! file.eof() ){
			std::stringstream sline;
			std::getline(file, line);
			sline << line;

			std::string type;
			sline >> type;

			if (type.compare("v") == 0)
			{
				float x;
				float y;
				float z;
				sline >> x >> y >> z;

				D3DXVECTOR3 vertex(x, y, -z);
				vertices.push_back(vertex);
			}

			if (type.compare("vn") == 0)
			{
				float x;
				float y;
				float z;
				sline >> x >> y >> z;

				D3DXVECTOR3 normal(x, y, z);
				normals.push_back(normal);
			}

			if (type.compare("vt") == 0)
			{
				float x;
				float y;
				sline >> x >> y;

				D3DXVECTOR2 texc(x, y);
				textureCoords.push_back(texc);
			}

			if (type.compare("f") == 0)
			{
				std::string inds1;
				std::string inds2;
				std::string inds3;

				sline >> inds1 >> inds2 >> inds3;
				Strings strings;

				Indices3 i30;
				Indices3 i31;
				Indices3 i32;

				Split(inds1, '/', strings);
				i30.vertex   = atoi(strings[0].c_str());
				i30.texcoord = atoi(strings[1].c_str());
				i30.normal   = atoi(strings[2].c_str());
				
				strings.clear();

				Split(inds2, '/', strings);
				i31.vertex   = atoi(strings[0].c_str());
				i31.texcoord = atoi(strings[1].c_str());
				i31.normal   = atoi(strings[2].c_str());
				strings.clear();

				Split(inds3, '/', strings);
				i32.vertex   = atoi(strings[0].c_str());
				i32.texcoord = atoi(strings[1].c_str());
				i32.normal   = atoi(strings[2].c_str());
				strings.clear();

				indices3.push_back(i30);
				indices3.push_back(i32);
				indices3.push_back(i31);
			}
		}
		LOG("Complete");

		file.close();
		return true;
	}
	return false;
}