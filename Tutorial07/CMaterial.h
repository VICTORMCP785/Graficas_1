#pragma once
#include "CRenderTarget.h"

class CMaterial
{
	/*
	CMaterial
	{
		Tiene que obtener los materiales de assimp
		Tiene que hacer el proceso de carga del material

		unsigned int ID;
		Lista de texturas (clase texture);
	};
	*/

public:
	CMaterial();
	~CMaterial();

	unsigned int m_ID;
	CTexture2D m_Texture;

	void init(unsigned int ID, CTexture2D Texture);
private:

};

