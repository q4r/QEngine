#include "MD5Parser.h"

#include "Globals.h"

#define CALLIF(x, y, func) if (x.compare(y) == 0) { if ( ! func(file) ) { return false; }  continue; }
#define CHECK(x, y) if (x != y) { LOG(x << " != " << y); return false; };
#define CHECKSTR(x, y, msg) if (x.compare(y) != 0) { LOG(msg); return false; };

#define SKIPLINE { 	std::string line; std::getline(file, line); }

MD5Parser::MD5Parser() :
	numJoints(0),
	numMeshes(0),
	numFrames(0),
	frameRate(0),
	numAnimatedComponents(0),
	joints(NULL)
{
}


MD5Parser::~MD5Parser(){
	SAFEDELETEARRAY(joints);
	SAFEDELETEARRAY(meshes);	
}

bool MD5Parser::Init(const std::string& path, const std::string meshName){
	std::string meshFileName = path + "\\" + meshName + ".md5mesh";
	std::string animFileName = path + "\\" + meshName + ".md5anim";

	std::ifstream meshFile;
	meshFile.open(meshFileName);


	if ( ! meshFile.is_open() ){
		return false;
	}

	if ( ! ParseMeshFile(meshFile) ){
		meshFile.close();
		return false;
	}
	meshFile.close();

	std::ifstream animFile;
	animFile.open(animFileName);

	if ( animFile.is_open() ){
		if ( ! ParseAnimFile(animFile) ){
			animFile.close();
			return false;
		}
	}
	return true;
}

bool MD5Parser::ParseMeshFile(std::ifstream& file){
	std::string name;

	while (file >> name){
		CALLIF(name, "MD5Version", Version);
		CALLIF(name, "commandline", CommandLine);
		CALLIF(name, "numJoints", NumJointsMesh);
		CALLIF(name, "numMeshes", NumMeshesMesh);

		CALLIF(name, "joints", JointsMesh);

		CALLIF(name, "mesh", MeshMesh);
		return true;
	}

	return true;
}

bool MD5Parser::ParseAnimFile(std::ifstream& file){
	std::string name;

	while (file >> name){
		CALLIF(name, "MD5Version", Version);
		CALLIF(name, "commandline", CommandLine);
		CALLIF(name, "numJoints", NumJointsAnim);
		CALLIF(name, "numFrames", NumFramesAnim);
		CALLIF(name, "frameRate", FrameRateAnim);
		CALLIF(name, "numAnimatedComponents", NumAnimatedComponentsAnim);
		
		CALLIF(name, "hierarchy", HierarchyAnim);
		return true;
	}

	return true;
}

//-----------------------------------------------------------

bool MD5Parser::Version(std::ifstream& file){
	int i;
	file >> i;
	CHECK(i, 10);
	SKIPLINE;
	return true;
}

bool MD5Parser::CommandLine(std::ifstream& file){
	SKIPLINE;
	return true;
}

bool MD5Parser::NumJointsMesh(std::ifstream& file){
	file >> numJoints;
	SKIPLINE;
	if (numJoints <= 0){
		return false;
	}

	joints = new Joint[numJoints];
	return true;
}

bool MD5Parser::NumJointsAnim(std::ifstream& file){
	int i;
	file >> i;
	CHECK(numJoints, i);
	SKIPLINE;
	return true;
}

bool MD5Parser::NumMeshesMesh(std::ifstream& file){
	file >> numMeshes;
	
	meshes = new Mesh[numMeshes];

	SKIPLINE;
	return true;
}

bool MD5Parser::NumFramesAnim(std::ifstream& file){
	file >> numFrames;
	SKIPLINE;
	return true;
}

bool MD5Parser::FrameRateAnim(std::ifstream& file){
	file >> frameRate;
	SKIPLINE;
	return true;
}

bool MD5Parser::NumAnimatedComponentsAnim(std::ifstream& file){
	file >> numAnimatedComponents;
	SKIPLINE;
	return true;
}

//-------------------------------------------------------------------------------

bool MD5Parser::JointsMesh(std::ifstream& file){
	if (!joints){
		return false;
	}

	char ch;
	file >> ch;
	CHECK(ch, '{');
	SKIPLINE;

	for (unsigned int i = 0; i < numJoints; i++){
		file >> joints[i].name;
		file >> joints[i].parent;
		file >> ch;
		CHECK(ch, '(');
		file >> joints[i].pos.x >> joints[i].pos.y >> joints[i].pos.z;
		file >> ch;
		CHECK(ch, ')');

		file >> ch;
		CHECK(ch, '(');

		file >> joints[i].ori.x >> joints[i].ori.y >> joints[i].ori.z;
		file >> ch;
		CHECK(ch, ')');
		SKIPLINE;
	}

	file >> ch;
	CHECK(ch, '}');
	SKIPLINE;
	return true;
}

bool MD5Parser::HierarchyAnim(std::ifstream& file){
	if (!joints){
		return false;
	}

	char ch;
	file >> ch;
	CHECK(ch, '{');
	SKIPLINE;

	for (unsigned int i = 0; i < numJoints; i++){
		std::string name;
		file >> name;
		CHECKSTR(name, joints[i].name, "Error: Joint name");

		int parent;
		file >> parent;
		CHECK(parent, joints[i].parent);

		file >> joints[i].flags;
		file >> joints[i].startIndex;

		SKIPLINE;
	}

	file >> ch;
	CHECK(ch, '}');
	SKIPLINE;
	return true;
}

bool MD5Parser::MeshMesh(std::ifstream& file){
	if (!meshes){
		return false;
	}

	static unsigned int meshNumder = 0;

	char ch;
	file >> ch;
	CHECK(ch, '{');
	SKIPLINE;

	std::string name;
	file >> name;
	CHECKSTR(name, "shader", "Error: shader");
	file >> meshes[meshNumder].shader;
	SKIPLINE;

	//------------------------------------
	file >> name;	
	CHECKSTR(name, "numverts", "Error: numverts");

	unsigned int numverts;
	file >> numverts;
	meshes[meshNumder].verts.reserve(numverts);
	SKIPLINE;

	for (unsigned int i = 0; i < numverts; i++){
		file >> name;
		CHECKSTR(name, "vert", "Error: vert");
		unsigned int verti;
		file >> verti;
		CHECK(verti, i);

		file >> ch;
		CHECK(ch, '(');

		Mesh::Vert vert;
		file >> vert.u >> vert.v;

		file >> ch;
		CHECK(ch, ')');
		file >> vert.startW >> vert.countW;
		SKIPLINE;

		meshes[meshNumder].verts.push_back(vert);
	}

	//------------------------------------
	file >> name;	
	CHECKSTR(name, "numtris", "Error: numtris");
	unsigned int numtris;
	file >> numtris;
	meshes[meshNumder].tris.reserve(numtris);
	SKIPLINE;

	for (unsigned int i = 0; i < numtris; i++){
		file >> name;
		CHECKSTR(name, "tri", "Error: tri");
		unsigned int trii;
		file >> trii;
		CHECK(trii, i);

		Mesh::Tri tri;
		file >> tri.i1 >> tri.i2 >> tri.i3;
		SKIPLINE;

		meshes[meshNumder].tris.push_back(tri);
	}

	//------------------------------------
	file >> name;	
	CHECKSTR(name, "numweights", "Error: numweights");
	unsigned int numweights;
	file >> numweights;
	meshes[meshNumder].weights.reserve(numweights);
	SKIPLINE;

	for (unsigned int i = 0; i < numweights; i++){
		file >> name;
		CHECKSTR(name, "weight", "Error: weight");
		unsigned int weighti;
		file >> weighti;
		CHECK(weighti, i);

		Mesh::Weight weight;
		file >> weight.joint >> weight.bias;

		file >> ch;
		CHECK(ch, '(');

		file >> weight.pos.x >> weight.pos.y >> weight.pos.z;

		file >> ch;
		CHECK(ch, ')');
		SKIPLINE;

		meshes[meshNumder].weights.push_back(weight);
	}

	file >> ch;
	CHECK(ch, '}');
	SKIPLINE;

	meshNumder++;

	LOG("mesh #" << meshNumder);
	return true;
}