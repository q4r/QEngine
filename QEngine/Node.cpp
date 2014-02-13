#include "Node.h"


Node::Node() : 
	parent(NULL),
	position(0, 0, 0),
	orientation()
	{
}


Node::~Node(){
}

void Node::SetParent(Node* _parent){
	parent = _parent;
}

void Node::SetPosition(D3DXVECTOR3 _position){
	position = _position;
}

void Node::SetPosition(float x, float y, float z){
	position.x = x;
	position.y = y;
	position.z = z;
}

void Node::SetOrientation(const Quaternion& _orientation){
	orientation = _orientation;
}

void Node::SetOrientation(float x, float y, float z, float w){
	orientation.Set(x, y, z, w);
}

void Node::SetOrientation(D3DXVECTOR3 vector, float angle){
	float halfAngle = angle / 2.0;
	float sinAngle = sin(halfAngle);
	orientation.Set(vector.x * sinAngle, vector.y * sinAngle, vector.z * sinAngle, cos(halfAngle));
}

D3DXVECTOR3 Node::GetLocalPosition(){
	return position;
}

Quaternion Node::GetLocalOrientation(){
	return orientation;
}

D3DXVECTOR3 Node::GetGlobalPosition(){
	if (parent){
		return parent->GetGlobalPosition() + GetGlobalOrientation().Rotate(position);
	}else{
		return position;
	}
}

Quaternion Node::GetGlobalOrientation(){
	if (parent){
		return parent->GetGlobalOrientation().Product(orientation);
	}else{
		return orientation;
	}
}

D3DXVECTOR3 Node::Transform(D3DXVECTOR3 point){
	point = GetGlobalOrientation().Rotate(point);
	point += GetGlobalPosition();
	return point;
}