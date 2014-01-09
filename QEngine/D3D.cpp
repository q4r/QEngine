#include "D3D.h"
#include "Globals.h"

//=======================================================================================================

D3D::D3D(void) :
	pSwapChain(NULL),
	pDevice(NULL),
	pContext(NULL),
	pRenderTargetView(NULL),
	pDepthStencilBuffer(NULL),
	pDepthStencilState(NULL),
	pDepthStencilView(NULL){
	LOG("D3D: constructor");
}


D3D::~D3D(void){
	SAFERELEASE(pDepthStencilView);
	SAFERELEASE(pDepthStencilState);
	SAFERELEASE(pDepthStencilBuffer);
	SAFERELEASE(pRenderTargetView);
	SAFERELEASE(pContext);
	SAFERELEASE(pDevice);
	SAFERELEASE(pSwapChain);	

	LOG("D3D: destructor");
}


bool D3D::Init(HWND hwnd, int screenWidth, int screenHeight){
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	DXGI_SWAP_CHAIN_DESC swapChainDescr;

	ZeroMemory( &swapChainDescr, sizeof( swapChainDescr ) );
		
	swapChainDescr.OutputWindow = hwnd;
	swapChainDescr.Windowed = true;
	swapChainDescr.BufferCount = 1;
	swapChainDescr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescr.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDescr.BufferDesc.Width = screenWidth;
	swapChainDescr.BufferDesc.Height = screenHeight;
	swapChainDescr.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescr.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDescr.BufferDesc.RefreshRate.Denominator = 1;

	swapChainDescr.SampleDesc.Count = 1;
	swapChainDescr.SampleDesc.Quality = 0;

	if ( FAILED( D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, 
		&swapChainDescr, &pSwapChain, &pDevice, NULL, &pContext) ) ){
			LOG("D3D: Init: Error in D3D11CreateDeviceAndSwapChain");
			return false;
	}

	//back buffer
	ID3D11Texture2D* pBackBuffer;
	if ( FAILED( pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer) ) ){
		LOG("D3D: Init: Error in GetBuffer");
		return false;
	}

	if ( FAILED( pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView) ) ){
		LOG("D3D: Init: Error in CreateRenderTargetView");
		pBackBuffer->Release();
		return false;
	}

	pBackBuffer->Release();

	//depth buffer
	D3D11_TEXTURE2D_DESC depthBufferDescr;

	ZeroMemory(&depthBufferDescr, sizeof(depthBufferDescr));
	depthBufferDescr.Width = screenWidth;
	depthBufferDescr.Height = screenHeight;
	depthBufferDescr.MipLevels = 1;
	depthBufferDescr.ArraySize = 1;
	depthBufferDescr.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDescr.SampleDesc.Count = 1;
	depthBufferDescr.SampleDesc.Quality = 0;
	depthBufferDescr.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDescr.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDescr.CPUAccessFlags = 0;
	depthBufferDescr.MiscFlags = 0;

	if ( FAILED( pDevice->CreateTexture2D(&depthBufferDescr, NULL, &pDepthStencilBuffer) ) ){
		LOG("D3D: Init: Error in CreateTexture2D");
		return false;
	}

	//depth stensil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if ( FAILED( pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState) ) ){
		LOG("Error: CreateDepthStencilState\n");
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = depthBufferDescr.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if ( FAILED( pDevice->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView) ) ){
		LOG("Error: CreateDepthStencilView");
		return false;
	}

	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	//CreateRasterizerState

	//vewport
	D3D11_VIEWPORT viewport;

	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	pContext->RSSetViewports(1, &viewport);

	return true;
}

void D3D::Draw(Drawing* drawing){
	float clearColor[] = {0.3f, 0.3f, 0.3f, 1.0f};

	pContext->ClearRenderTargetView(pRenderTargetView, clearColor);
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	if (drawing){
		drawing();
	}

	pSwapChain->Present(0, 0);
}

ID3D11Device* D3D::GetDevice(){
	return pDevice;
}

ID3D11DeviceContext* D3D::GetContext(){
	return pContext;
}

void D3D::EnableDepthTest(bool state){
	//??????
}