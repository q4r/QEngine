#pragma once

#include "D3D.h"
#include "Scene.h"
#include <list>

class Window
{
public:
	typedef bool (Initialization)();	
	typedef bool (Logic)(void);
	typedef bool (Releasing)(void);

private:
	static Window* pWindow;
	HWND hWnd;

	D3D* d3d;

	Initialization* initialization;
	Logic* logic;
	D3D::Drawing* drawing;
	Releasing* releasing;

	typedef std::list<Scene*> Scenes;
	Scenes scenes;

private:
	Window();
	~Window(void);
	bool Init( HINSTANCE hInstance, int nCmdShow, unsigned int width, unsigned int height );

public:	
	static Window* GetWindow( HINSTANCE hInstance, int nCmdShow, unsigned int width, unsigned int height );
	static void Release();

	void SetInitFunc(Initialization* initialization);
	void SetLogicFunc(Logic* logic);
	void SetDrawFunc(D3D::Drawing* drawing);
	void SetReleaseFunc(Releasing* releasing);

	void Run();

	Scene* GetNewScene();
};



