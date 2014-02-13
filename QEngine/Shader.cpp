#include "Shader.h"
#include "Globals.h"

Shader::Shader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext) : 
	pDevice(_pDevice),
	pContext(_pContext),
	pVertexShader(0), 
	pPixelShader(0), 
	vsBuffer(0), 
	psBuffer(0), 
	pLayout(0), 
	matrixBuffer(0), 
	lightBuffer(0){
	LOG("Shader: constructor");
}


Shader::~Shader(void){
	SAFERELEASE(vsBuffer);
	SAFERELEASE(psBuffer);

	SAFERELEASE(matrixBuffer);
	SAFERELEASE(lightBuffer);
	LOG("Shader: destructor");
}

ID3D11VertexShader* Shader::InitVertexShader(const std::string& fileName, const std::string& funcName){
	ID3D11VertexShader* pVShader = NULL;
	ID3D10Blob* errBuffer;

	//vertex shader
	if ( FAILED( D3DX11CompileFromFileA(fileName.c_str(), NULL, NULL, funcName.c_str(), "vs_5_0",  D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vsBuffer, &errBuffer, NULL) ) ){
		if (errBuffer){
			LOG((LPCSTR)errBuffer->GetBufferPointer());
			SAFERELEASE(errBuffer);
		}else{
			LOG("Shader: Vertex shader file not found");
		}
		pVShader = NULL;
	}else{
		if ( FAILED( pDevice->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), NULL, &pVShader) ) ){
			pVShader = NULL;
			SAFERELEASE(vsBuffer);
		}
	}

	return pVShader;
}

ID3D11PixelShader* Shader::InitPixelShader(const std::string& fileName, const std::string& funcName){
	ID3D11PixelShader* pPShader;
	ID3D10Blob* errBuffer;

	//pixel shader
	if ( FAILED( D3DX11CompileFromFileA(fileName.c_str(), NULL, NULL, funcName.c_str(), "ps_5_0",  D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &psBuffer, &errBuffer, NULL) ) ){
		if (errBuffer){
			LOG((LPCSTR)errBuffer->GetBufferPointer());
			SAFERELEASE(errBuffer);
		}else{
			LOG("Shader: Pixel shader file not found");
		}
		pPShader = NULL;
	}else{
		if ( FAILED( pDevice->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), NULL, &pPShader) ) ){
			pPShader = NULL;
			SAFERELEASE(psBuffer);
		}
	}

	return pPShader;
}

bool Shader::ConstructInputLayout(unsigned int attributes){
	D3D11_INPUT_ELEMENT_DESC layout[5]; 
	
	D3D11_INPUT_ELEMENT_DESC position	= {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
	D3D11_INPUT_ELEMENT_DESC normal		= {"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
	D3D11_INPUT_ELEMENT_DESC color		= {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
	D3D11_INPUT_ELEMENT_DESC texcoord0	= {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
	D3D11_INPUT_ELEMENT_DESC texcoord1	= {"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};

	unsigned int attrCount = 1;
	layout[0] = position;

	if (attributes & AdditionalAttributes::NORMAL){
		layout[attrCount] = normal;
		attrCount++;
	}

	if (attributes & AdditionalAttributes::COLOR){
		layout[attrCount] = color;
		attrCount++;
	}

	if (attributes & AdditionalAttributes::TEXCOORD0){
		layout[attrCount] = texcoord0;
		attrCount++;
	}

	if (attributes & AdditionalAttributes::TEXCOORD1){
		layout[attrCount] = texcoord1;
		attrCount++;
	}
	
	if ( FAILED( pDevice->CreateInputLayout(layout, attrCount, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &pLayout) ) ){
		return false;
	}
	
	return true;
}

ID3D11Buffer* Shader::CreateShaderConstantsBuffer(unsigned int size){
	ID3D11Buffer* buffer;
	//Constants(uniforms)
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = size; // ======>>>> УВАГА!!! Значення завжди має ділитись на 16!!!!!!!!!
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (FAILED(pDevice->CreateBuffer(&bufferDesc, NULL, &buffer))){
		return NULL;
	}

	return buffer;
}


bool Shader::Init(const std::string& fileName, unsigned int additionalAttributes, const std::string& vertexFunctionName, const std::string& pixelFunctionName){

	LOG_("Shaders: Init... ");

	pVertexShader = InitVertexShader(fileName, vertexFunctionName);
	pPixelShader  = InitPixelShader (fileName, pixelFunctionName);

	if (! ConstructInputLayout(additionalAttributes) ){
		LOG("Error!");
		return false;
	}	

	matrixBuffer = CreateShaderConstantsBuffer(sizeof(MatrixBufferType));
	if (!matrixBuffer){
		LOG("Error!");
		return false;
	}

	lightBuffer = CreateShaderConstantsBuffer(sizeof(LightBufferType));
	if (!lightBuffer){
		LOG("Error!");
		return false;
	}

	LOG("OK!");
	return true;
}

bool Shader::SetShaderParameters(const D3DXMATRIX& _view, const D3DXMATRIX& _projection){
	D3D11_MAPPED_SUBRESOURCE mappedResources;
	MatrixBufferType* pMatrixData;
	LightBufferType* pLightData;
	
	if (FAILED(pContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources))){
		return false;
	}

	pMatrixData = (MatrixBufferType*)mappedResources.pData;

	pMatrixData->projection = _projection; 	
	pMatrixData->view = _view;	
	//pData->world = ?;
	D3DXMatrixIdentity(&pMatrixData->world);
	
	D3DXMatrixTranspose(&pMatrixData->projection, &pMatrixData->projection);
	D3DXMatrixTranspose(&pMatrixData->view, &pMatrixData->view);
	D3DXMatrixTranspose(&pMatrixData->world, &pMatrixData->world);

	pContext->Unmap(matrixBuffer, 0);

	//===========================

	if ( FAILED(pContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources)) ){
		return false;
	}
	pLightData = (LightBufferType*)mappedResources.pData;
	pLightData->diffuseColor = D3DXVECTOR4(1.0f, 0.8f, 0.8f, 1.0f);
	static float a = 0;
	a += 0.0001f;
	pLightData->lightDirection = D3DXVECTOR4(sin(a), 0.0f, cos(a), 0.0f);
	//pLightData->lightDirection = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.0f);

	pContext->Unmap(lightBuffer, 0);
	return true;
}

void Shader::Draw(Surface* surface, Camera* camera){	
	if ( ! surface ){
		return;
	}

	surface->SetAsCurrent();

	SetShaderParameters(camera->GetViewMatrix(), camera->GetProjectionMatrix());
	unsigned int bufferNumber = 0;
	pContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
	pContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);
	
	pContext->IASetInputLayout(pLayout);
	pContext->VSSetShader(pVertexShader, NULL, 0);
	pContext->PSSetShader(pPixelShader, NULL, 0);
	

	pContext->DrawIndexed(surface->GetIndexCount(), 0, 0);
}