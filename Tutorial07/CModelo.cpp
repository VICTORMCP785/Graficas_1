#include "CModelo.h"

CModelo::CModelo()
{
}

CModelo::~CModelo()
{
}

void CModelo::Init()
{
}

void CModelo::Set(CMesh Mesh)
{
	m_Meshes.push_back(Mesh);
}

CMesh CModelo::Get(unsigned int NumMesh)
{
	return m_Meshes[NumMesh];
}
