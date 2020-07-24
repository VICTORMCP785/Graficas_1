#pragma once
#include "CMesh.h"
#include <vector>
class CModelo
{
	/*
	CModelo
	{
		Lista de meshes
		Transform Propio
		get y set mesh
	};
	*/
public:
	CModelo();
	~CModelo();

	std::vector <CMesh> m_Meshes;
	CTransform m_Transform;

	void Init();

	void Set(CMesh Mesh);
	CMesh Get(unsigned int NumMesh);
private:
};

