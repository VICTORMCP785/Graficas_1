#pragma once
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"

class CTransform
{
public:
	CTransform();
	~CTransform();

	glm::vec3 m_Posicion;
	glm::vec3 m_Rotacion;
	glm::vec3 m_Escala;

	void init();
	void Update();
	void Destroy();
	void Render();

	void SetVecD();
	void SetVecF();
	void SetVecG();

	//VARIABLES
	//funcion set y get de cada 1
	//funcion get matrix genera y regresa la matrix
};

