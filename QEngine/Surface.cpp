#include "Surface.h"
#include "Globals.h"

#include <fstream>
#include <sstream>
#include <string>
#include <list>

Surface::Surface(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, Scene& _scene) : 
	pDevice(_pDevice), 
	pContext(_pContext), 
	pVertexBuffer(NULL), 
	pIndexBuffer(NULL), 
	vertexCount(0), 
	indexCount(0), 
	stride(0),
	texture(NULL),
	scene(_scene)
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

	pIndexBuffer = CreateBuffer(indices, sizeof(unsigned long), indexCount);

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

bool Surface::Init(Vertices& _vertices, TexCoords& _texCoords, Indices& _indices, Texture* _texture){
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

	texture = _texture;

	LOG("OK!");

	return result;
}

bool Surface::Init(Vertices& _vertices, Normals& _normals, TexCoords& _texCoords, Indices& _indices, Texture* _texture){
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

	texture = _texture;

	LOG("OK!");

	return result;
}


void Surface::SetAsCurrent(){

	if ( (pVertexBuffer) && (pIndexBuffer) )
	{
		if (texture){
			texture->SetAsCurrent();
		}

		unsigned int offset = 0;

		pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		pContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

unsigned int Surface::GetIndexCount(){
	return indexCount;
}

bool Surface::LoadFromObj(const std::string& fileName){
	std::ifstream file;

	file.open(fileName.c_str());

	if (file.is_open()){
		Surface::Vertices vertices;		
		Surface::Normals  normals;
		Surface::TexCoords texc;
		Surface::Indices  indices;

		std::vector<D3DXVECTOR3> verticesTemp;
		std::vector<D3DXVECTOR3>  normalsTemp;
		std::vector<D3DXVECTOR2> texcTemp;

		struct Indices3{
			unsigned int vertex;
			unsigned int texcoord;
			unsigned int normal;
		};

		std::list<Indices3> indices3;

		LOG_("Surface: Loading .OBJ...");
		std::string line;
		while ( ! file.eof() ){
			std::stringstream sline;
			std::getline(file, line);
			sline << line;

			std::string type;
			sline >> type;

			if (type.compare("v") == 0)
			{
				float x;
				float y;
				float z;
				sline >> x >> y >> z;

				D3DXVECTOR3 vertex(x, y, -z);
				verticesTemp.push_back(vertex);
			}

			if (type.compare("vn") == 0)
			{
				float x;
				float y;
				float z;
				sline >> x >> y >> z;

				D3DXVECTOR3 normal(x, y, z);
				normalsTemp.push_back(normal);
			}

			if (type.compare("vt") == 0)
			{
				float x;
				float y;
				sline >> x >> y;

				D3DXVECTOR2 texc(x, y);
				texcTemp.push_back(texc);
			}

			if (type.compare("f") == 0)
			{
				std::string inds1;
				std::string inds2;
				std::string inds3;

				sline >> inds1 >> inds2 >> inds3;
				Strings strings;

				Indices3 i30;
				Indices3 i31;
				Indices3 i32;

				Split(inds1, '/', strings);
				i30.vertex   = atoi(strings[0].c_str());
				i30.texcoord = atoi(strings[1].c_str());
				i30.normal   = atoi(strings[2].c_str());
				
				strings.clear();

				Split(inds2, '/', strings);
				i31.vertex   = atoi(strings[0].c_str());
				i31.texcoord = atoi(strings[1].c_str());
				i31.normal   = atoi(strings[2].c_str());
				strings.clear();

				Split(inds3, '/', strings);
				i32.vertex   = atoi(strings[0].c_str());
				i32.texcoord = atoi(strings[1].c_str());
				i32.normal   = atoi(strings[2].c_str());
				strings.clear();

				indices3.push_back(i30);
				indices3.push_back(i32);
				indices3.push_back(i31);
			}
		}
		LOG("Complete");

		LOG_("Surface: Create lists... ");
		unsigned int index = 0;
		for (std::list<Indices3>::iterator i = indices3.begin(); i != indices3.end(); i++){
			vertices.push_back(verticesTemp[i->vertex - 1]);
			if (normalsTemp.size() > 0){
				normals.push_back(normalsTemp[i->normal - 1]);
			}
			if (texcTemp.size() > 0){
				texc.push_back(texcTemp[i->texcoord - 1]);
			}
			indices.push_back(index++);
		}
		LOG("Complete");

		if ( ! Init(vertices, texc, indices, NULL) ) {
			return false;
		}

		file.close();
		return true;
	}
	return false;
}

void Surface::SetTexture(Texture* _texture){
	texture = _texture;
}

Texture* Surface::GetTexture(){
	return texture;
}

Scene& Surface::GetScene(){
	return scene;
}