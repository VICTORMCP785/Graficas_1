#pragma once
#include "Snake.h"
#include "VertexBuffer.h"
#include "CIndexBuffer.h"
#include "CTransform.h"
#include "CMaterial.h"
#include "resource.h"

class CMesh
{
public:
	CMesh();
	~CMesh();

	CTransform m_Transformacion;		//transformaciones en el mundo g_world
	CVertexBuffer m_VertexBuffer;		//vertex buffer
	CIndexBuffer m_IndexBuffer;			//index buffer
	CMaterial m_Material;				//Material
#ifdef D3D11
	D3D_PRIMITIVE_TOPOLOGY m_Topologia;	//Topologia
#endif
	void init();
	void Update();
	void Destroy();
	void Render();
private:
};

//Como generar un rayo