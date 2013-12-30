#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10math.h>

class Camera{
	friend class Scene;
private:
	//view 
	D3DXVECTOR3 eye;	//position
	D3DXVECTOR3 dir;	//direction
	D3DXVECTOR3 up;		//direction

	//projection
	float nearPlane;
	float farPlane;
	float fovY;
	float aspect;

	float speed;
	bool invertY;

	Camera();
	~Camera();
public:

	void UpDown(float dt);
	void ForwardBackward(float dt);
	void LeftRight(float dt);

	void RotUpDown(float dt);
	void RotLeftRight(float dt);

	void SetPosition(float x, float y, float z);
	void LookAt(float x, float y, float z);

	void SetAspect(float aspect);

	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjectionMatrix();
};

