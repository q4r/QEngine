#pragma once

#include "Surface.h"
#include "Shader.h"
#include "Texture.h"
#include <list>

class Mesh{
	friend class Scene;
private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;

	struct DrawElement{
		Surface* pSurface;
		Shader* pShader;
		Texture* pTexture;
	};

	typedef std::list<DrawElement> Surfaces;
	Surfaces surfaces;

	Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~Mesh();

public:
	bool AddSurface(Surface::Vertices& vertices, Surface::Colors& colors, Surface::Indices& indices, Shader* pShader, Texture* pTexture);
	bool AddSurface(Surface::Vertices& vertices, Surface::TexCoords& texCoords, Surface::Indices& indices, Shader* pShader, Texture* pTexture);
	bool AddSurface(Surface::Vertices& vertices, Surface::Normals& normals, Surface::TexCoords& texCoords, Surface::Indices& indices, Shader* pShader, Texture* pTexture);

	void Draw(const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& projection);
};

