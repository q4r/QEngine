#pragma once

#include "Quaternion.h"
#include "Mesh.h"
#include "Camera.h"

#include <list>

class Node{
	friend class Scene;
	friend class Skeleton;

	Node* parent;
	D3DXVECTOR3 position;
	Quaternion orientation;
	D3DXVECTOR3 scale;
	
	typedef std::list<Node*> Children;
	Children children;

	Mesh* mesh;

	Node();
	~Node();

	void SetParent(Node* parent);
public:

	Node* CreateChild();	

	void SetPosition(D3DXVECTOR3 position);
	void SetPosition(float x, float y, float z);

	void SetOrientation(const Quaternion& orientation);
	void SetOrientation(D3DXVECTOR3 vector, float angle);
	void SetOrientation(float x, float y, float z, float w);

	void SetScale(float x, float y, float z);
	void SetScale(float s);

	D3DXVECTOR3 GetLocalPosition();
	Quaternion GetLocalOrientation();
	D3DXVECTOR3 GetLocalScale();

	D3DXVECTOR3 GetGlobalPosition();
	Quaternion GetGlobalOrientation();

	D3DXMATRIX GetLocalMatrix();
	D3DXMATRIX GetGlobalMatrix();
	//D3DXVECTOR3 Transform(D3DXVECTOR3 point);

	void SetMesh(Mesh* mesh);

	void Draw(Camera& camera);
};

