#pragma once

#include <fstream>
#include <string>

#include <vector>

class MD5Parser{
friend class MD5Loader;

private:
	bool ParseMeshFile(std::ifstream& file);
	bool ParseAnimFile(std::ifstream& file);

	bool Version(std::ifstream& file);
	bool CommandLine(std::ifstream& file);
	bool NumJointsMesh(std::ifstream& file);
	bool NumJointsAnim(std::ifstream& file);
	bool NumMeshesMesh(std::ifstream& file);
	bool NumFramesAnim(std::ifstream& file);
	bool FrameRateAnim(std::ifstream& file);
	bool NumAnimatedComponentsAnim(std::ifstream& file);

	bool JointsMesh(std::ifstream& file);
	bool HierarchyAnim(std::ifstream& file);

	bool MeshMesh(std::ifstream& file);

	bool BoundsAnim(std::ifstream& file);
	bool BaseframeAnim(std::ifstream& file);
	bool FrameAnim(std::ifstream& file);
	

	unsigned int numJoints;
	unsigned int numMeshes;
	unsigned int numFrames;
	unsigned int frameRate;
	unsigned int numAnimatedComponents;

	struct Joint{
		std::string name;
		int parent;

		struct{
			float x, y, z;
		} pos, ori;

		struct{
			float x, y, z;
		} basePos, baseOri;

		unsigned int flags;
		unsigned int startIndex;
	};
	Joint* joints;

	struct Mesh{
		std::string shader;

		struct Vert{
			float u, v;
			unsigned int startW;
			unsigned int countW;
		};
		std::vector<Vert> verts;

		struct Tri{
			unsigned int i1, i2, i3;
		};
		std::vector<Tri> tris;

		struct Weight{
			unsigned int joint;
			float bias;
			struct{
				float x, y, z;
			} pos;

		};
		std::vector<Weight> weights;
	};
	Mesh* meshes;
	
	struct Bounds{
		struct{
			float x, y, z;
		} p1, p2;
	};
	Bounds* bounds;

	std::vector<float*> frames;
public:
	MD5Parser();
	~MD5Parser();

	bool Init(const std::string& path, const std::string meshName);

};

