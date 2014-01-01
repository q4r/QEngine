#include "Texture.h"
#include "Globals.h"

Texture::Texture(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext) : 
	pDevice(_pDevice),
	pContext(_pContext),
	texture(NULL), 
	sampleState(NULL){	
	LOG("Texture: constructor");
}

Texture::~Texture(void){
	SAFERELEASE(sampleState);
	SAFERELEASE(texture);
	LOG("Texture: destructor");
}

bool Texture::Init(const std::string& fileName){
	LOG_("Texture: Init... ");
	if (FAILED( D3DX11CreateShaderResourceViewFromFileA(pDevice, fileName.c_str(), NULL, NULL, &texture, NULL) )){
		LOG("Error");
		texture = NULL;
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	if( FAILED(pDevice->CreateSamplerState(&samplerDesc, &sampleState)) ){
		LOG("Error");
		sampleState = NULL;
		SAFERELEASE(texture);
		return false;
	}

	LOG("OK");
	return true;
}

void Texture::SetAsCurrent(){
	pContext->PSSetShaderResources(0, 1, &texture);
	pContext->PSSetSamplers(0, 1, &sampleState);
}