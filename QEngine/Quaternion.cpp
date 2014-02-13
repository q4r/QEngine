#include "Quaternion.h"

#include <cmath>


Quaternion::Quaternion() : 
	x(0.0f), 
	y(0.0f),
	z(0.0f),
	w(1.0f)
{
}

Quaternion::Quaternion(const Quaternion& q) : 
	x(q.x), 
	y(q.y),
	z(q.z),
	w(q.w)
{
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w) : 
	x(_x),
	y(_y),
	z(_z), 
	w(_w)
{
}

Quaternion::Quaternion(const D3DXVECTOR3& vector, float angle):
	x(vector.x),
	y(vector.y),
	z(vector.z), 
	w(0)
{
	Mul(sin(angle / 2));
	w = cos(angle / 2);
}

Quaternion::~Quaternion(){
}

void Quaternion::Set(float _x, float _y, float _z, float _w){
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Quaternion::SetAsIdentity(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;

	w = 1.0f - x * x - y * y - z * z;
	w = (w <= 0.0f) ? 0.0f : -sqrt(w);
}

void Quaternion::Add(const Quaternion& q){
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
}

void Quaternion::Sub(const Quaternion& q){
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
}

void Quaternion::Mul(float f){
	x *= f;
	y *= f;
	z *= f;
	w *= f;
}

void Quaternion::Div(float f){
	x /= f;
	y /= f;
	z /= f;
	w /= f;
}

Quaternion Quaternion::Product(const Quaternion& q){
	//Quaternion result;
	//D3DXVECTOR3 v1(x, y, z);
	//D3DXVECTOR3 v2(q.x, q.y, q.z);

	//D3DXVECTOR3 v3;

	//D3DXVec3Cross(&v3, &v1, &v2);
	//v3 = v3 + v1 * q.w + v2 * w;

	//result.x = v3.x;
	//result.y = v3.y;
	//result.z = v3.z;

	//result.w = w * q.w - D3DXVec3Dot(&v1, &v2);

	//return result;

	float _w = (w * q.w) - (x * q.x) - (y * q.y) - (z * q.z);
	float _x = (x * q.w) + (w * q.x) + (y * q.z) - (z * q.y);
	float _y = (y * q.w) + (w * q.y) + (z * q.x) - (x * q.z);
	float _z = (z * q.w) + (w * q.z) + (x * q.y) - (y * q.x);

	return Quaternion(_x, _y, _z, _w);

	//Quaternion another(q);
	//Quaternion temp;
 //   temp.w = w * another.w - x * another.x - y * another.y - z * another.z;
 //   temp.x = w * another.x + x * another.w + y * another.z - z * another.y;
 //   temp.y = w * another.y - x * another.z + y * another.w + z * another.x;
 //   temp.z = w * another.z + x * another.y - y * another.x + z * another.w;
 //   
 //   return temp;
}

float Quaternion::GetNorm(){
	return x * x + y * y + z * z + w * w;
}

float Quaternion::GetMagn(){
	return sqrt(GetNorm());
}

Quaternion Quaternion::GetConjugate(){
	return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::GetInverse(){
	Quaternion q(GetConjugate());
	q.Div(GetNorm());
	return q;
}

void Quaternion::Identity(){
	Div(GetNorm());
}

D3DXVECTOR3 Quaternion::Rotate(const D3DXVECTOR3& vector){
	Quaternion v(vector.x, vector.y, vector.z, 0.0f);

	Quaternion q = Product(v);
	q = q.Product(GetInverse());

	return D3DXVECTOR3(q.x, q.y, q.z);
}