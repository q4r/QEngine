#pragma once

#include <d3d11.h>
#include <D3DX11tex.h>
#include <string>


class Texture{
	friend class Scene;
private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;

	ID3D11ShaderResourceView* texture;
	ID3D11SamplerState* sampleState;

	Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~Texture(void);

	bool Init(const std::string& fileName);
public:	
	void SetAsCurrent();
};

