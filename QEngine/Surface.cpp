#include "Surface.h"
#include "Globals.h"

Surface::Surface(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext) : 
	pDevice(_pDevice), 
	pContext(_pContext), 
	pVertexBuffer(NULL), 
	pIndexBuffer(NULL), 
	vertexCount(0), 
	indexCount(0), 
	stride(0)
{
	LOG("Surface: constructor");
}

Surface::~Surface(void){	
	SAFERELEASE(pVertexBuffer);
	SAFERELEASE(pIndexBuffer);
	LOG("Surface: destructor");
}


ID3D11Buffer* Surface::CreateBuffer(void* data, unsigned int sizeofDataElement, unsigned int elementCount){
	ID3D11Buffer* pBuffer = NULL;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subResData;

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeofDataElement * elementCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	subResData.pSysMem = data;
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;

	if ( FAILED(pDevice->CreateBuffer(&bufferDesc, &subResData, &pBuffer) ) ){
		return NULL;
	}
	return pBuffer;
}

bool Surface::SetIndices(Indices& _indices){
	indexCount = _indices.size();
	unsigned long* indices = new unsigned long[indexCount];
	unsigned int i = 0;

	for(Indices::iterator it = _indices.begin(); it != _indices.end(); ++it){
		indices[i] = *it;
		i++;
	}

	pIndexBuffer = CreateBuffer(indices, sizeof(unsigned long), indexCount);//ÁÀÃ!!!!!!!!!! D3D11_BIND_VERTEX_BUFFER ìàº áóòè INDEX

	delete[] indices;

	return ( pIndexBuffer != NULL );
}

bool Surface::Init(Vertices& _vertices, Colors& _colors, Indices& _indices){
	bool result = true;
	LOG_("Surface: Init... ");
	vertexCount = _vertices.size();
	VertexTypeColor* vertices = new VertexTypeColor[vertexCount];
	
	unsigned int i = 0;
	for(Vertices::iterator it = _vertices.begin(); it != _vertices.end(); ++it){
		vertices[i].position = D3DXVECTOR4(it->x, it->y, it->z , 1.0f);
		i++;
	}

	i = 0;
	for(Colors::iterator it = _colors.begin(); it != _colors.end(); ++it){
		vertices[i].color = D3DXVECTOR4(it->x, it->y, it->z, 1.0f);
		i++;
	}

	stride = sizeof(VertexTypeColor);
	pVertexBuffer = CreateBuffer(vertices, stride, vertexCount);

	delete[] vertices;

	if ( ! pVertexBuffer ){
		result = false;
	}else{
		result = SetIndices(_indices);
	}

	LOG("OK!");

	return result;
}

bool Surface::Init(Vertices& _vertices, TexCoords& _texCoords, Indices& _indices){
	bool result = true;
	LOG_("Surface: Init... ");

	vertexCount = _vertices.size();
	VertexTypeTexture* vertices = new VertexTypeTexture[vertexCount];
	
	unsigned int i = 0;
	for(Vertices::iterator it = _vertices.begin(); it != _vertices.end(); ++it){
		vertices[i].position = D3DXVECTOR4(it->x, it->y, it->z , 1.0f);
		i++;
	}

	i = 0;
	for(TexCoords::iterator it = _texCoords.begin(); it != _texCoords.end(); ++it){
		vertices[i].texture = *it;
		i++;
	}

	stride = sizeof(VertexTypeTexture);
	pVertexBuffer = CreateBuffer(vertices, stride, vertexCount);

	delete[] vertices;

	if ( ! pVertexBuffer ){
		result = false;
	}else{
		result = SetIndices(_indices);
	}

	LOG("OK!");

	return result;
}

bool Surface::Init(Vertices& _vertices, Normals& _normals, TexCoords& _texCoords, Indices& _indices){
	bool result = true;
	LOG_("Surface: Init... ");
	vertexCount = _vertices.size();
	if ( (vertexCount != _normals.size()) || (vertexCount != _texCoords.size()) ){
		LOG("Error");
		return false;
	}
	
	VertexTypeTextureNormal* vertices = new VertexTypeTextureNormal[vertexCount];

	Vertices::iterator itVert = _vertices.begin();
	Normals::iterator itNorm = _normals.begin();
	TexCoords::iterator itTexc = _texCoords.begin();

	for (unsigned int i = 0; i < vertexCount; i++){
		vertices[i].position = D3DXVECTOR4(itVert->x, itVert->y, itVert->z , 1.0f);
		vertices[i].normal = D3DXVECTOR4(itNorm->x, itNorm->y, itNorm->z , 0.0f);
		vertices[i].texture = *itTexc;
		itVert++;
		itNorm++;
		itTexc++;
	}

	stride = sizeof(VertexTypeTextureNormal);
	pVertexBuffer = CreateBuffer(vertices, stride, vertexCount);

	delete[] vertices;

	if ( ! pVertexBuffer ){
		result = false;
	}else{
		result = SetIndices(_indices);
	}

	LOG("OK!");

	return result;
}


void Surface::Draw(){

	if ( (pVertexBuffer) && (pIndexBuffer) )
	{
		unsigned int offset = 0;

		pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		pContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pContext->DrawIndexed(indexCount, 0, 0);
	}
}
