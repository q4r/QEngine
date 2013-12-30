#pragma once

#include <string>

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10math.h>

#include "Camera.h"
#include "Texture.h"
#include "Surface.h"

class Shader{
	friend class Scene;
public:
	enum AdditionalAttributes : unsigned {
		NORMAL		= 1 << 0,
		COLOR		= 1 << 1,
		TEXCOORD0	= 1 << 2,
		TEXCOORD1	= 1 << 3
	};

private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;

	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;

	ID3D10Blob* vsBuffer;
	ID3D10Blob* psBuffer;

	ID3D11InputLayout* pLayout;

	struct MatrixBufferType{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightBufferType{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR4 lightDirection;
	};

	ID3D11Buffer* matrixBuffer;	
	ID3D11Buffer* lightBuffer;	

	ID3D11VertexShader* InitVertexShader(const std::string& fileName, const std::string& funcName);
	ID3D11PixelShader*  InitPixelShader (const std::string& fileName, const std::string& funcName);

	bool ConstructInputLayout(unsigned int attributes);

	ID3D11Buffer* CreateShaderConstantsBuffer(unsigned int size);

	Shader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~Shader(void);

	

	bool SetShaderParameters(const D3DXMATRIX& _view, const D3DXMATRIX& _projection);

	bool Init(const std::string& fileName,
		unsigned int additionalAttributes,
		const std::string& vertexFunctionName = "VMain", const std::string& pixelFunctionName = "PMain");	
public:
	void Draw(Surface* surface, Camera* camera, Texture* texture);
};

