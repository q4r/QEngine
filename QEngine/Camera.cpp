#include "Camera.h"
#include "Globals.h"

#include <string>

Camera::Camera(void) : 
	eye(0.0f, 0.0f, -1.0f), 
	dir(0.0f, 0.0f, 1.0f), 
	up(0.0f, 1.0f, 0.0f), 
	nearPlane(0.1f), 
	farPlane(100.0f), 
	fovY(45.0f), 
	aspect(1.0f){
	LOG("Camera: constructor\n");
}


Camera::~Camera(void){
	LOG("Camera: destructor\n");
}

void Camera::UpDown(float dt){
	eye += up * dt;
}

void Camera::ForwardBackward(float dt){
	eye += dir * dt;
}

void Camera::LeftRight(float dt){
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &dir, &up);

	eye += right * dt;
}

void Camera::RotUpDown(float dt){
	//TODO: rotate dir
	D3DXVec3Normalize(&dir, &dir);
}

void Camera::RotLeftRight(float dt){
	//TODO: rotate dir
	D3DXVec3Normalize(&dir, &dir);
}

void Camera::SetPosition(float x, float y, float z){
	eye.x = x;
	eye.y = y;
	eye.z = z;
}

void Camera::LookAt(float x, float y, float z){
	D3DXVECTOR3 at(x, y, z);
	dir = at - eye;
	D3DXVec3Normalize(&dir, &dir);
}

void Camera::SetAspect(float _aspect){
	aspect = _aspect;
}

D3DXMATRIX Camera::GetViewMatrix(){
	D3DXMATRIX matrix;	
	D3DXVECTOR3 target = eye + dir;
	D3DXMatrixLookAtLH(&matrix, &eye, &target, &up);
	return matrix;
}

D3DXMATRIX Camera::GetProjectionMatrix(){
	D3DXMATRIX matrix;	
	D3DXMatrixPerspectiveFovLH(&matrix, fovY, aspect, nearPlane, farPlane);
	return matrix;
}