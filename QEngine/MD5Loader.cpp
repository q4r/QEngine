#include "MD5Loader.h"

#include "MD5Parser.h"
#include "Globals.h"

MD5Loader::MD5Loader(){
}

MD5Loader::~MD5Loader(){
}

Mesh* MD5Loader::Init(Scene& scene, const std::string& path, const std::string meshName, Shader* shader){
	MD5Parser parser;
	if ( ! parser.Init(path, meshName) ){
		return false;
	}

	Mesh* pMesh = scene.CreateMesh();

	for (unsigned int meshi = 0; meshi < parser.numMeshes; meshi++){
		Surface::Vertices vertices;
		Surface::TexCoords texCoords;
		Surface::Indices indises;

		MD5Parser::Mesh& mesh = parser.meshes[meshi];		

		unsigned int vertCount = mesh.verts.size();
		vertices.reserve(vertCount);
		texCoords.reserve(vertCount);
		for (unsigned int i = 0; i < vertCount; i++){
			D3DXVECTOR3 vertex(0.0f, 0.0f, 0.0f);
			MD5Parser::Mesh::Vert& vert = mesh.verts[i];
		
			texCoords.push_back(D3DXVECTOR2(vert.u, vert.v));

			unsigned int weightStart = vert.startW;
			unsigned int weightCount = vert.countW;

			for (unsigned int j = 0; j < weightCount; j++){
				MD5Parser::Mesh::Weight& weight = mesh.weights[weightStart + j];
				MD5Parser::Joint& joint = parser.joints[weight.joint];

				D3DXVECTOR3 weightPos(weight.pos.x, weight.pos.y, weight.pos.z);

				D3DXVECTOR3 jointPos(joint.pos.x, joint.pos.y, joint.pos.z);
				Quaternion jointOri;
				jointOri.SetAsIdentity(joint.ori.x, joint.ori.y, joint.ori.z);

				weightPos = jointOri.Rotate(weightPos);
				weightPos += jointPos;
				weightPos *= weight.bias;

				vertex += weightPos;
			}
			vertex.z *= -1;
			vertices.push_back(vertex);
		}

		unsigned int triCount = mesh.tris.size();
		for (unsigned int i = 0; i < triCount; i++){
			MD5Parser::Mesh::Tri& tri = mesh.tris[i];
			indises.push_back(tri.i1);
			indises.push_back(tri.i2);
			indises.push_back(tri.i3);
		}

		mesh.shader.erase(0, 1);
		mesh.shader.erase(mesh.shader.size() - 1, 1);
		unsigned int found = mesh.shader.find_last_of("/\\");
		std::string textureFileName = path + "/" + mesh.shader.substr(found + 1);

		Texture* texture = scene.CreateTextureFromFile(textureFileName);
		if ( ! texture ){
			LOG(textureFileName);
			return false;
		}

		pMesh->AddSurface(vertices, texCoords, indises, shader, texture);
	}

	return pMesh;
}