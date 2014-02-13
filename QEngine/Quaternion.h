#pragma once

#include <D3DX10math.h>

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
};

