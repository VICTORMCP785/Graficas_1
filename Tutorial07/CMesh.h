#pragma once
#include "Snake.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include "VertexBuffer.h"
#include "CIndexBuffer.h"
#include "resource.h"

class CMesh
{
public:
	CMesh();
	~CMesh();

	//VARIABLES
	glm::mat4 m_Transformacion;			//transformaciones en el mundo g_world
#ifdef D3D11
	CVertexBuffer m_VertexBuffer;		//vertex buffer
	CIndexBuffer m_Indexbuffer;			//index buffer
	D3D_PRIMITIVE_TOPOLOGY Topologia;	//Topologia
#endif
	

	void init();
	void Update();
	void Destroy();
	void Render();
private:
};

