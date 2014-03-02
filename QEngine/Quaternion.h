#pragma once

#include <D3D11.h>
#include <D3DX10math.h>

struct D3DXVECTOR3;
struct D3DXMATRIX;

class Quaternion{
	float x, y, z, w;
public:
	Quaternion();	
	Quaternion(const Quaternion& q);
	Quaternion(float x, float y, float z, float w); 
	Quaternion(const D3DXVECTOR3& vector, float angle);
	~Quaternion();

	void Set(float x, float y, float z, float w);
	void SetAsIdentity(float x, float y, float z);

	void Add(const Quaternion& q);
	void Sub(const Quaternion& q);
	void Mul(float q);
	void Div(float q);

	Quaternion Product(const Quaternion& q);

	float GetNorm();
	float GetMagn();

	Quaternion GetConjugate();
	Quaternion GetInverse();

	void Identity();

	D3DXVECTOR3 Rotate(const D3DXVECTOR3& vector);
	D3DXMATRIX GetMatrix();
};

