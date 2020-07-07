#pragma once
#include "Snake.h"
#include "VertexBuffer.h"
#include "CIndexBuffer.h"
#include "CTransform.h"
#include "resource.h"

class CMesh
{
public:
	CMesh();
	~CMesh();

	//VARIABLES
	//Cambiar Transformacion con la nueva clase transformacion que contenga
	//vec 3 pos
	//vec 3 rotacion
	//vec 3 escala
	//funcion set y get de cada 1
	//funcion get matrix genera y regresa la matrix
	glm::mat4 m_Transformacion;			//transformaciones en el mundo g_world
	CVertexBuffer m_VertexBuffer;		//vertex buffer
	CIndexBuffer m_IndexBuffer;			//index buffer
#ifdef D3D11
	D3D_PRIMITIVE_TOPOLOGY m_Topologia;	//Topologia
#endif
	void init();
	void Update();
	void Destroy();
	void Render();
private:
};

