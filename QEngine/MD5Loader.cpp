#include "MD5Loader.h"

#include "Globals.h"
#include "Scene.h"


#include <iostream>
#include <sstream>
#include <limits> 

//#define MAX = std::numeric_limits<std::streamsize>::max();
#define MAX 1024

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
	numMeshes(0){
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
	unsigned int meshIndex = 0;

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
			joints = new Node[numJoints];
			continue;
		}

		if (element.compare("numMeshes") == 0){
			file >> numMeshes;
			meshes = new _Mesh[numMeshes];
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
			CHECK_RET(LoadMesh(file, meshIndex++), "Mesh parsing error");
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
	
	std::string name;
	unsigned int parent;
	float x, y, z;
	Quaternion orientation;

	for (unsigned int i = 0; i < count; i++){
		file >> name >> parent;
		file >> ch >> x >> y >> z >> ch;
		joints[i].SetPosition(x, y, z);

		file >> ch >> x >> y >> z >> ch;
		orientation.SetAsIdentity(x, y, z);
		joints[i].SetOrientation(orientation);

		file.ignore(MAX, '\n');
	}

	file >> ch;
	CHECK_RET(ch == '}', "} is not found");

	return true;
}

bool MD5Loader::LoadMesh(std::ifstream& file, unsigned int meshIndex){
	CHECK_RET(meshes != NULL, "File corrupted");

	LOG_("Mesh# " << meshIndex + 1 << " ");

	char ch;
	file >> ch;

	CHECK_RET(ch == '{', "{ is not found");
	
	while ( file.good() ){
		std::string element;
		file >> element;

		if (element.compare("shader") == 0){
			GET_STRING_RET(meshes[meshIndex].shader);
			continue;
		}

		if (element.compare("numverts") == 0){
			unsigned int cnt;
			file >> cnt;
			CHECK_RET(cnt != 0, "Count of vertives is 0");

			meshes[meshIndex].numverts = cnt;
			meshes[meshIndex].verts = new Vert[cnt];

			LoadVerts(file, cnt, meshes[meshIndex]);

			continue;
		}

		if (element.compare("numtris") == 0){
			unsigned int cnt;
			file >> cnt;
			CHECK_RET(cnt != 0, "Count of triangles is 0");

			meshes[meshIndex].numtris = cnt;
			meshes[meshIndex].tris = new Tri[cnt];

			LoadTris(file, cnt, meshes[meshIndex]);

			continue;
		}

		if (element.compare("numweights") == 0){
			unsigned int cnt;
			file >> cnt;
			CHECK_RET(cnt != 0, "Count of weights is 0");

			meshes[meshIndex].numweights = cnt;
			meshes[meshIndex].weights = new Weight[cnt];

			LoadWeights(file, cnt, meshes[meshIndex]);

			continue;
		}

		if (element.compare("}") == 0){
			break;
		}

		LOG("Unknown element: " << element);
		break;
	}
	
	LOG("OK");
	
	return true;
}

bool MD5Loader::LoadVerts(std::ifstream& file, unsigned int count, _Mesh& mesh){
	char ch;
	for (unsigned int i = 0; i < count; i++){
		std::string elem;
		file >> elem;
		CHECK_RET(elem.compare("vert") == 0, "It is not vert");

		file >> mesh.verts[i].index;
		file >> ch >> mesh.verts[i].uv.u >> mesh.verts[i].uv.v >> ch;
		file >> mesh.verts[i].startWeight >> mesh.verts[i].countWeight;
		file.ignore(MAX, '\n');
	}

	return true;
}

bool MD5Loader::LoadTris(std::ifstream& file, unsigned int count, _Mesh& mesh){
	for (unsigned int i = 0; i < count; i++){
		std::string elem;
		file >> elem;
		CHECK_RET(elem.compare("tri") == 0, "It is not tri");

		file >> mesh.tris[i].index >> mesh.tris[i].i0 >> mesh.tris[i].i1 >> mesh.tris[i].i2;

		file.ignore(MAX, '\n');
	}

	return true;
}

bool MD5Loader::LoadWeights(std::ifstream& file, unsigned int count, _Mesh& mesh){
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

bool MD5Loader::GetSurface(unsigned int index, Surface& surface, std::string& shader){

	Surface::Vertices vertices;
	Surface::TexCoords texCoords;
	Surface::Normals normals;
	Surface::Indices indices;

	CHECK_RET(index < numMeshes, "GetMesh: Invalid index value");
	
	_Mesh& mesh = meshes[index];

	unsigned int vertCnt = mesh.numverts;

	vertices.reserve(vertCnt);
	texCoords.reserve(vertCnt);
	normals.reserve(vertCnt);

	for (unsigned int i = 0; i < vertCnt; i++){
		unsigned int start = mesh.verts[i].startWeight;
		unsigned int weightCnt = mesh.verts[i].countWeight;

		D3DXVECTOR2 texCoord(0.0f, 0.0f);
		texCoord.x = mesh.verts[i].uv.u;
		texCoord.y = mesh.verts[i].uv.v;
		texCoords.push_back(texCoord);

		D3DXVECTOR3 vertexPos(0.0f, 0.0f, 0.0f);
		for (unsigned int j = 0; j < weightCnt; j++){
			Weight& weight = mesh.weights[start + j];
			Node& joint = joints[weight.joint];

			D3DXVECTOR3 p = joint.GetLocalOrientation().Rotate(weight.pos);
			vertexPos += (joint.GetLocalPosition() + p) * weight.bias;
		}

		float yz = vertexPos.y;
		vertexPos.y = vertexPos.z;
		vertexPos.z = yz;

		vertices.push_back(vertexPos);

		D3DXVECTOR3 vertexNorm(0.0f, 0.0f, 0.0f);
		normals.push_back(vertexNorm);
	}

	unsigned int indCnt = meshes[index].numtris;
	for (unsigned int i = 0; i < indCnt; i++){
		Tri& tri = mesh.tris[i];
		indices.push_back(tri.i0);
		indices.push_back(tri.i1);
		indices.push_back(tri.i2);

		D3DXVECTOR3 v1 = vertices[tri.i0] - vertices[tri.i1];
		D3DXVECTOR3 v2 = vertices[tri.i2] - vertices[tri.i1];	
		D3DXVec3Normalize(&v1, &v1);
		D3DXVec3Normalize(&v2, &v2);

		D3DXVECTOR3 norm;
		D3DXVec3Cross(&norm, &v2, &v1);
		D3DXVec3Normalize(&norm, &norm);

		normals[tri.i0] += norm;
		normals[tri.i1] += norm;
		normals[tri.i2] += norm;
	}

	for (unsigned int i = 0; i < vertCnt; i++){
		D3DXVec3Normalize(&normals[i], &normals[i]);
	}

	if ( ! surface.Init(vertices, normals, texCoords, indices) ){
		return false;
	}
	shader = mesh.shader;

	return true;
}

unsigned int MD5Loader::GetSurfaceCount(){
	return numMeshes;
}