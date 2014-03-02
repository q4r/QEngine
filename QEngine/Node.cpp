#include "Node.h"

#define GTOR (3.141592f / 180.0f)

Node::Node() : 
	parent(NULL),
	position(0, 0, 0),
	orientation(),
	children()
	{
}


Node::~Node(){
	for (Children::iterator i = children.begin(); i != children.end(); i++){
		delete (*i);
	}
	children.clear();
}

Node* Node::CreateChild(){
	Node* child = new Node();
	child->SetParent(this);
	children.push_back(child);
	return child;
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
	float halfAngle = (angle * GTOR) / 2.0f ;
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

D3DXMATRIX Node::GetLocalMatrix(){
	D3DXMATRIX matrix = orientation.GetMatrix();
	matrix.m[3][0] = position.x;
	matrix.m[3][1] = position.y;
	matrix.m[3][2] = position.z;

	return matrix;
}

D3DXMATRIX Node::GetGlobalMatrix(){
	D3DXMATRIX matrix = GetGlobalOrientation().GetMatrix();
	D3DXVECTOR3 pos = GetGlobalPosition();
	matrix.m[3][0] = pos.x;
	matrix.m[3][1] = pos.y;
	matrix.m[3][2] = pos.z;

	return matrix;
}

//D3DXVECTOR3 Node::Transform(D3DXVECTOR3 point){
//	point = GetGlobalOrientation().Rotate(point);
//	point += GetGlobalPosition();
//	return point;
//}

void Node::SetMesh(Mesh* _mesh){
	mesh = _mesh;
}

void Node::Draw(Camera& camera){
	mesh->Draw(GetGlobalMatrix(), camera.GetViewMatrix(), camera.GetProjectionMatrix());

	for (Children::iterator i = children.begin(); i != children.end(); i++){
		(*i)->Draw(camera);
	}
}