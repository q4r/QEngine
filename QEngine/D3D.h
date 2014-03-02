#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dxgi.h>
#include <D3Dcommon.h>
#include <D3D11.h>

class D3D
{
	friend class Window;
public:
	typedef void (Drawing)(void);
private:
	IDXGISwapChain* pSwapChain;

	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	ID3D11RenderTargetView* pRenderTargetView;
	ID3D11Texture2D* pDepthStencilBuffer;
	ID3D11DepthStencilState* pDepthStencilState;
	ID3D11DepthStencilView* pDepthStencilView;

private:
	D3D(void);
	~D3D(void);
	
	bool Init(HWND hwnd, int screenWidth, int screenHeight);
	void Draw(Drawing* drawing);

public:
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	void EnableDepthTest(bool state);
};

