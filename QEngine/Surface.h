#pragma once

#include <D3D11.h>
#include <D3DX10math.h>

#include <list>
#include <vector>

class Surface
{
	friend class Mesh;
public:
	typedef std::vector<D3DXVECTOR3> Vertices;
	typedef std::vector<D3DXVECTOR3> Normals;
	typedef std::vector<D3DXVECTOR3> Colors;
	typedef std::vector<D3DXVECTOR2> TexCoords;
	typedef std::list<unsigned long> Indices;

private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;

	struct VertexTypeColor{
		D3DXVECTOR4 position;
		D3DXVECTOR4 color;
	};

	struct VertexTypeTexture{
		D3DXVECTOR4 position;
		D3DXVECTOR2 texture;
	};

	struct VertexTypeTextureNormal{
		D3DXVECTOR4 position;
		D3DXVECTOR4 normal;
		D3DXVECTOR2 texture;
	};

	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;

	unsigned int vertexCount;
	unsigned int indexCount;
	unsigned int stride;

	ID3D11Buffer* CreateBuffer(void* data, unsigned int sizeofDataElement, unsigned int elementCount);
	bool SetIndices(Indices& indices);

	Surface(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~Surface(void);	
public:
	bool Init(Vertices& vertices, Colors& colors, Indices& indices);
	bool Init(Vertices& vertices, TexCoords& texCoords, Indices& indices);
	bool Init(Vertices& vertices, Normals& normals, TexCoords& texCoords, Indices& indices);

	void Draw();
};

