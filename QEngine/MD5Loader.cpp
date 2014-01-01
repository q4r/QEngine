#include "MD5Loader.h"

#include "Globals.h"
#include <iostream>
#include <sstream>
#include <limits> 

#define MAX std::numeric_limits<std::streamsize>::max()

#define GET_STRING_RET(str)if ( ! GetString(file, str) ){return false;}
#define CHECK_RET(con, msg) if (!(con)) { LOG("MD5: " << msg); return false; };

bool GetString(std::ifstream& file, std::string& str){
	str.clear();
	char ch;

	file >> ch;
	if (ch != '"'){
		str = ch;
		return false;
	}

	file.get(ch);
	while (ch != '"'){
		str += ch;
		file.get(ch);
		if (!file.good()){
			return false;
		}
	}

	return true;
}

MD5Loader::MD5Loader() :
	joints(NULL),
	meshes(NULL),
	version(0),
	numJoints(0),
	numMeshes(0),
	constLoadedMesh(0){
}

MD5Loader::~MD5Loader(void){
	SAFEDELETEARRAY(joints);

	if (meshes){
		for (unsigned int i = 0; i < numMeshes; i++){
			SAFEDELETEARRAY(meshes[i].verts);
			SAFEDELETEARRAY(meshes[i].tris);
			SAFEDELETEARRAY(meshes[i].weights);
		}
		SAFEDELETEARRAY(meshes);
	}
}

bool MD5Loader::Init(const std::string& fileName){
	std::ifstream file;
	file.open(fileName.c_str());

	if ( ! file.is_open() ){
		return false;
	}

	while ( file.good() ){
		std::string element;
		file >> element;

		if (element.compare("MD5Version") == 0){
			file >> version;
			CHECK_RET(version == 10, "Incompatible version of MD5");
			continue;
		}

		if (element.compare("commandline") == 0){
			std::string commandline;
			GET_STRING_RET(commandline);
			continue;
		}

		if (element.compare("numJoints") == 0){
			file >> numJoints;
			joints = new Joint[numJoints];
			continue;
		}

		if (element.compare("numMeshes") == 0){
			file >> numMeshes;
			meshes = new Mesh[numMeshes];
			for (unsigned int i = 0; i < numMeshes; i++){
				meshes[i].verts = NULL;
				meshes[i].tris = NULL;
				meshes[i].weights = NULL;
			}
			continue;
		}
		
		if (element.compare("joints") == 0){			
			CHECK_RET(numJoints != 0, "Joints count is 0");
			CHECK_RET(LoadJoints(file, numJoints), "Joints parsing error");
			continue;
		}
		
		if (element.compare("mesh") == 0){			
			CHECK_RET(numMeshes != 0, "Meshes count is 0");
			CHECK_RET(LoadMesh(file), "Mesh parsing error");
			continue;
		}

	}

	return true;
}

bool MD5Loader::LoadJoints(std::ifstream& file, unsigned int count){
	CHECK_RET(joints != NULL, "File corrupted");

	char ch;
	file >> ch;

	CHECK_RET(ch == '{', "{ is not found");
	
	for (unsigned int i = 0; i < count; i++){
		file >> joints[i].name >> joints[i].parent;
		file >> ch >> joints[i].pos.x >> joints[i].pos.y >> joints[i].pos.z >> ch;
		file >> ch >> joints[i].orient.x >> joints[i].orient.y >> joints[i].orient.z >> ch;
		file.ignore(MAX, '\n');
	}

	file >> ch;
	CHECK_RET(ch == '}', "} is not found");

	return true;
}

bool MD5Loader::LoadMesh(std::ifstream& file){
	CHECK_RET(meshes != NULL, "File corrupted");

	LOG_("Mesh# " << constLoadedMesh + 1 << " ");

	char ch;
	file >> ch;

	CHECK_RET(ch == '{', "{ is not found");
	
	while ( file.good() ){
		std::string element;
		file >> element;

		if (element.compare("shader") == 0){
			GET_STRING_RET(meshes[constLoadedMesh].shader);
			continue;
		}

		if (element.compare("numverts") == 0){
			unsigned int cnt;
			file >> cnt;
			CHECK_RET(cnt != 0, "Count of vertives is 0");

			meshes[constLoadedMesh].numverts = cnt;
			meshes[constLoadedMesh].verts = new Vert[cnt];

			LoadVerts(file, cnt, meshes[constLoadedMesh]);

			continue;
		}

		if (element.compare("numtris") == 0){
			unsigned int cnt;
			file >> cnt;
			CHECK_RET(cnt != 0, "Count of triangles is 0");

			meshes[constLoadedMesh].numtris = cnt;
			meshes[constLoadedMesh].tris = new Tri[cnt];

			LoadTris(file, cnt, meshes[constLoadedMesh]);

			continue;
		}

		if (element.compare("numweights") == 0){
			unsigned int cnt;
			file >> cnt;
			CHECK_RET(cnt != 0, "Count of weights is 0");

			meshes[constLoadedMesh].numweights = cnt;
			meshes[constLoadedMesh].weights = new Weight[cnt];

			LoadWeights(file, cnt, meshes[constLoadedMesh]);

			continue;
		}

		if (element.compare("}") == 0){
			break;
		}

		LOG("Unknown element: " << element);
		break;
	}
	
	LOG("OK");
	constLoadedMesh++;
	
	return true;
}

bool MD5Loader::LoadVerts(std::ifstream& file, unsigned int count, Mesh& mesh){
	char ch;
	for (unsigned int i = 0; i < count; i++){
		std::string elem;
		file >> elem;
		CHECK_RET(elem.compare("vert") == 0, "It is not vert");

		file >> mesh.verts[i].index;
		file >> ch >> mesh.verts[i].uv.u >> mesh.verts[i].uv.v >> ch;
		file >> mesh.verts[i].startWeight >> mesh.verts[i].startWeight;
		file.ignore(MAX, '\n');
	}

	return true;
}

bool MD5Loader::LoadTris(std::ifstream& file, unsigned int count, Mesh& mesh){
	char ch;
	for (unsigned int i = 0; i < count; i++){
		std::string elem;
		file >> elem;
		CHECK_RET(elem.compare("tri") == 0, "It is not tri");

		file >> mesh.tris[i].index >> mesh.tris[i].i0 >> mesh.tris[i].i1 >> mesh.tris[i].i2;

		file.ignore(MAX, '\n');
	}

	return true;
}

bool MD5Loader::LoadWeights(std::ifstream& file, unsigned int count, Mesh& mesh){
	char ch;
	for (unsigned int i = 0; i < count; i++){
		std::string elem;
		file >> elem;
		CHECK_RET(elem.compare("weight") == 0, "It is not weight");

		file >> mesh.weights[i].index >> mesh.weights[i].joint >> mesh.weights[i].bias;
		file >> ch >> mesh.weights[i].pos.x >> mesh.weights[i].pos.y >> mesh.weights[i].pos.z >> ch;

		file.ignore(MAX, '\n');
	}

	return true;
}