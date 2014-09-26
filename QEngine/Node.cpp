#include "Node.h"

#define GTOR (3.141592f / 180.0f)

Node::Node() : 
	parent(NULL),
	position(0, 0, 0),
	scale(1, 1, 1),
	orientation(),
	children(),
	mesh(NULL)
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
	//float halfAngle = (angle * GTOR) / 2.0f ;
	//float sinAngle = sin(halfAngle);
	//orientation.Set(vector.x * sinAngle, vector.y * sinAngle, vector.z * sinAngle, cos(halfAngle));
	
	orientation.Set(vector, GTOR * angle);
}

void Node::SetScale(float _x, float _y, float _z){
	scale.x = _x;
	scale.y = _y;
	scale.z = _z;
}

void Node::SetScale(float _s){
	scale.x = _s;
	scale.y = _s;
	scale.z = _s;
}


D3DXVECTOR3 Node::GetLocalPosition(){
	return position;
}

Quaternion Node::GetLocalOrientation(){
	return orientation;
}

D3DXVECTOR3 Node::GetLocalScale(){
	return scale;
}

D3DXVECTOR3 Node::GetGlobalPosition(){
	if (parent){
		return parent->GetGlobalPosition() + parent->GetGlobalOrientation().Rotate(position);
		//return parent->GetGlobalPosition() + GetGlobalOrientation().Rotate(position);
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
	D3DXMATRIX scaleMatrix;
	D3DXMatrixIdentity(&scaleMatrix);
	//scaleMatrix.m[0][0] = scale.x;
	//scaleMatrix.m[1][1] = scale.y;
	//scaleMatrix.m[2][2] = scale.z;

	D3DXMATRIX matrix = GetGlobalOrientation().GetMatrix();
	//D3DXMatrixIdentity(&matrix);

	D3DXMatrixMultiply(&matrix, &scaleMatrix, &matrix);

	D3DXMATRIX translationMatrix;
	D3DXMatrixIdentity(&translationMatrix);

	D3DXVECTOR3 pos = GetGlobalPosition();
	//translationMatrix.m[3][0] = pos.x;
	//translationMatrix.m[3][1] = pos.y;
	//translationMatrix.m[3][2] = pos.z;

	matrix.m[3][0] = pos.x;
	matrix.m[3][1] = pos.y;
	matrix.m[3][2] = pos.z;


//	D3DXMatrixMultiply(&matrix, &translationMatrix, &matrix);


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
	if (mesh){
		mesh->Draw(GetGlobalMatrix(), camera.GetViewMatrix(), camera.GetProjectionMatrix());
	}

	for (Children::iterator i = children.begin(); i != children.end(); i++){
		(*i)->Draw(camera);
	}
}