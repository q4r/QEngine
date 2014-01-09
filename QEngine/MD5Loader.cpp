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

void SetW(D3DXVECTOR4& q){
	float t = 1.0f - (q.x * q.x) - (q.y * q.y) - (q.z * q.z);
	if (t < 0.0f)
	{
		q.w = 0.0f;
	}
	else
	{
		q.w = -sqrt (t);
	}
}

D3DXVECTOR4 QuatMult(D3DXVECTOR4 qa, D3DXVECTOR4 qb){
	D3DXVECTOR4 r;
	r.w = (qa.w * qb.w) - (qa.x * qb.x) - (qa.y * qb.y) - (qa.z * qb.z);
	r.x = (qa.x * qb.w) + (qa.w * qb.x) + (qa.y * qb.z) - (qa.z * qb.y);
	r.y = (qa.y * qb.w) + (qa.w * qb.y) + (qa.z * qb.x) - (qa.x * qb.z);
	r.z = (qa.z * qb.w) + (qa.w * qb.z) + (qa.x * qb.y) - (qa.y * qb.x);
	return r;
}

Pos Rotate(Pos quaternion, Pos point){
	D3DXVECTOR4 quat(quaternion.x, quaternion.y, quaternion.z, 0.0f);
	SetW(quat);
	D3DXVECTOR4 pnt(point.x, point.y, point.z, 0.0f);
	
	pnt = QuatMult(quat, pnt);
	quat.x *= -1;
	quat.y *= -1;
	quat.z *= -1;
	pnt = QuatMult(pnt, quat);
	point.x = pnt.x;
	point.y = pnt.y;
	point.z = pnt.z;
	return point;
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
			joints = new Joint[numJoints];
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
	//Surface::Normals normals;
	Surface::Indices indices;

	CHECK_RET(index < numMeshes, "GetMesh: Invalid index value");
	
	_Mesh& mesh = meshes[index];

	unsigned int vertCnt = mesh.numverts;
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
			Joint& joint = joints[weight.joint];

			Pos p = Rotate(joint.orient, weight.pos);

			vertexPos.x += (joint.pos.x + p.x) * weight.bias;
			vertexPos.z += (joint.pos.y + p.y) * weight.bias;
			vertexPos.y += (joint.pos.z + p.z) * weight.bias;
		}
		vertices.push_back(vertexPos);
	}

	unsigned int indCnt = meshes[index].numtris;
	for (unsigned int i = 0; i < indCnt; i++){
		Tri& tri = mesh.tris[i];
		indices.push_back(tri.i0);
		indices.push_back(tri.i1);
		indices.push_back(tri.i2);
	}

	if ( ! surface.Init(vertices, texCoords, indices, NULL) ){
		return false;
	}
	shader = mesh.shader;

	return true;
}

unsigned int MD5Loader::GetSurfaceCount(){
	return numMeshes;
}