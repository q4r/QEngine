#include "Globals.h"
#include "Mesh.h"

#include "MD5Loader.h"

Mesh::Mesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext) :
	pDevice(_pDevice), 
	pContext(_pContext),
	surfaces()
{
}

Mesh::~Mesh(){
	for (Surfaces::iterator surface = surfaces.begin(); surface != surfaces.end(); surface++){
		surface->pShader = NULL;
		surface->pTexture = NULL;
		SAFEDELETE(surface->pSurface);
	}
	surfaces.clear();
}

bool Mesh::AddSurface(Surface::Vertices& vertices, Surface::Colors& colors, Surface::Indices& indices, Shader* pShader, Texture* pTexture){
	Surface* pSurface = new Surface(pDevice, pContext);

	if (! pSurface->Init(vertices, colors, indices)){
		delete pSurface;
		return false;
	}

	DrawElement element;
	element.pSurface = pSurface;
	element.pShader = pShader;
	element.pTexture = pTexture;

	surfaces.push_back(element);

	return true;
}

bool Mesh::AddSurface(Surface::Vertices& vertices, Surface::TexCoords& texCoords, Surface::Indices& indices, Shader* pShader, Texture* pTexture){
	Surface* pSurface = new Surface(pDevice, pContext);

	if (! pSurface->Init(vertices, texCoords, indices)){
		delete pSurface;
		return false;
	}

	DrawElement element;
	element.pSurface = pSurface;
	element.pShader = pShader;
	element.pTexture = pTexture;

	surfaces.push_back(element);

	return true;
}

bool Mesh::AddSurface(Surface::Vertices& vertices, Surface::Normals& normals, Surface::TexCoords& texCoords, Surface::Indices& indices, Shader* pShader, Texture* pTexture){
	Surface* pSurface = new Surface(pDevice, pContext);

	if (! pSurface->Init(vertices, normals, texCoords, indices)){
		delete pSurface;
		return false;
	}

	DrawElement element;
	element.pSurface = pSurface;
	element.pShader = pShader;
	element.pTexture = pTexture;

	surfaces.push_back(element);

	return true;
}

void Mesh::Draw(const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& projection){
	for (Surfaces::iterator surface = surfaces.begin(); surface != surfaces.end(); surface++){
		if (!surface->pShader){
			LOG("Error: Need shader!!!");
			continue;
		}
		surface->pShader->SetShaderParameters(world, view, projection);
		surface->pShader->SetAsCurrent();

		if (surface->pTexture){
			surface->pTexture->SetAsCurrent();
		}

		surface->pSurface->Draw();
	}	
}