#pragma once

#include "Quaternion.h"

class Node{
	Node* parent;
	D3DXVECTOR3 position;
	Quaternion orientation;
	//D3DXVECTOR3 scale;
public:
	Node();
	~Node();

	void SetParent(Node* parent);

	void SetPosition(D3DXVECTOR3 position);
	void SetPosition(float x, float y, float z);

	void SetOrientation(const Quaternion& orientation);
	void SetOrientation(D3DXVECTOR3 vector, float angle);
	void SetOrientation(float x, float y, float z, float w);

	D3DXVECTOR3 GetLocalPosition();
	Quaternion GetLocalOrientation();

	D3DXVECTOR3 GetGlobalPosition();
	Quaternion GetGlobalOrientation();

	D3DXVECTOR3 Transform(D3DXVECTOR3 point);
private:

};

