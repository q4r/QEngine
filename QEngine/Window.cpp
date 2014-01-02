#include "Window.h"
#include "Globals.h"


Window* Window::pWindow = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch( message ){
		case WM_DESTROY:{
			PostQuitMessage( 0 );
			break;
		}
		case WM_CLOSE:{
			PostQuitMessage( 0 );
			break;
		}
		default:{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

//=======================================================================================================

Window::Window() : 
	hWnd(0), 
	d3d(NULL), 
	initialization(NULL), 
	logic(NULL), 
	drawing(NULL), 
	releasing(NULL),
	scenes(){
	LOG("Window: constructor");
}

Window::~Window(void){
	for (Scenes::iterator i = scenes.begin(); i != scenes.end(); i++){
		delete *i;
	}
	SAFEDELETE(d3d);
	LOG("Window: destructor");
}

bool Window::Init( HINSTANCE hInstance, int nCmdShow, unsigned int width, unsigned int height ){
	#define CLASSNAME L"DX11Tutorials"

    WNDCLASSEX wc;
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, ( LPCTSTR )IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_CROSS );
	wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASSNAME;
    wc.hIconSm = LoadIcon( NULL, ( LPCTSTR )IDI_APPLICATION );

    if( !RegisterClassEx( &wc ) )
        return false;

    RECT rc = { 0, 0, width, height };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    hWnd = CreateWindow( CLASSNAME, L"QEngine", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( ! hWnd )
        return false;

	d3d = new D3D();
	if ( ! d3d->Init(hWnd, width, height) ){
		 LOG("Error: init D3D");
		return false;
	}

	ShowWindow( hWnd, nCmdShow );

	return true;
}

//=======================================================================================================

Window* Window::GetWindow( HINSTANCE hInstance, int nCmdShow, unsigned int width, unsigned int height ){
	if ( ! pWindow ){
		START_LOGING;
		pWindow = new Window();

		if ( ! pWindow->Init(hInstance, nCmdShow, width, height) ){
			LOG("Error: window init");
			pWindow->Release();
		}
	}

	return pWindow;
}

void Window::Run(){
	MSG msg;

	if (initialization){
		if ( !initialization() ){
			LOG("Error: Initialization.");
			Release();
			return;
		}
	}
	
	LOG("Window: >>>");
	do{
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			if (logic){
				logic();
			}
			if (d3d){
				d3d->Draw(drawing);
			}
		}

	}while (msg.message != WM_QUIT);
	LOG("Window: <<<");

	if (releasing && (!releasing())){
		LOG("Error: Releasing.");
	}
}

void Window::Release(){
	SAFEDELETE(pWindow);
	STOP_LOGING;
}

void Window::SetInitFunc(Initialization* _initialization){
	initialization = _initialization;
}

void Window::SetLogicFunc(Logic* _logic){
	logic = _logic;
}

void Window::SetDrawFunc(D3D::Drawing* _drawing){
	drawing = _drawing;
}

void Window::SetReleaseFunc(Releasing* _releasing){
	releasing = _releasing;
}

Scene* Window::GetNewScene(){
	Scene* scene = new Scene(*d3d);
	scenes.push_back(scene);
	return scene;
}