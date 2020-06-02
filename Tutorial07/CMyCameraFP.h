#pragma once
#include "CMyCamara.h"
class CMyCameraFP : public CMyCamara
{
private:

public:
	CMyCameraFP();

	void move();
	void getKeyPress(WPARAM key);
	void getKeyRelease(WPARAM key);
	void rotateFront(glm::vec3 Dir);
	void rotateRight(glm::vec3 Dir);

	float m_maxFrontRotation;
	float m_maxRightRotation;
	float m_RightRotation;
	float m_FrontRotation;

};

