#include "CMaterial.h"

CMaterial::CMaterial()
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::init(unsigned int ID, CTexture2D Texture)
{
	m_ID = ID;
	m_Texture = Texture;
}

