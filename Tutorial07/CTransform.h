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

	glm::vec3 D;

	void init();
	void Update();
	void Destroy();
	void Render();
};

