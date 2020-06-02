#include "CMyCameraFP.h"
CMyCameraFP::CMyCameraFP()
{
	m_maxFrontRotation = 20.f;
	m_maxRightRotation = 45.f;
	m_FrontRotation = 0.f;
	m_RightRotation = 0.f;
}

void CMyCameraFP::move()
{
	if (mForward)
	{
		F.Position += (GetFront() * F.Movement);
	}
	if (mBack)
	{
		F.Position += (GetFront() * -F.Movement);
	}

	if (mLeft)
	{
		F.Position += (GetRigth() * -F.Movement);
	}
	if (mRight)
	{
		F.Position += (GetRigth() * F.Movement);
	}

	glm::mat4 Axis
	{
		F.Rigth.x,	F.Rigth.y,	F.Rigth.z, 0,
		UpRef.x,	UpRef.y,	UpRef.z,   0,
		F.Front.x,	F.Front.y,	F.Front.z, 0,
		0,			0,			0,			1
	};

	glm::mat4 Pos
	{
		1, 0, 0, -F.Position.x,
		0, 1, 0, -F.Position.y,
		0, 0, 1, -F.Position.z,
		0, 0, 0, 1
	};

	Pos *= Axis;

	ViewMatrix = Pos;
	UpdateViewMatrix();
}

void CMyCameraFP::getKeyPress(WPARAM key)
{
	//Z Movement
	if (key == 'W')
	{
		mForward = true;
	}
	else if (key == 'S')
	{
		mBack = true;
	}
	//X Movement
	if (key == 'A')
	{
		mLeft = true;
	}
	else if (key == 'D')
	{
		mRight = true;
	}

	if (key == VK_LEFT)
	{
		mRotateLeft = true;
	}
	else if (key == VK_RIGHT)
	{
		mRotateRight = true;
	}
}

void CMyCameraFP::getKeyRelease(WPARAM key)
{
	if (key == 'W')
	{
		mForward = false;
	}
	else if (key == 'S')
	{
		mBack = false;
	}
	if (key == 'A')
	{
		mLeft = false;
	}
	else if (key == 'D')
	{
		mRight = false;
	}
	if (key == VK_LEFT)
	{
		mRotateLeft = false;
	}
	else if (key == VK_RIGHT)
	{
		mRotateRight = false;
	}
}

void CMyCameraFP::rotateFront(glm::vec3 Dir)
{
	float camcos = cosf(Dir.z / 100.f);
	float camsin = sinf(Dir.z / 100.f);

	if (Dir.z < 0)
	{
		if ((m_FrontRotation + Dir.z) >= -m_maxFrontRotation)
		{
			m_FrontRotation += Dir.z;
			glm::mat4 RotZ
			{
				camcos, -camsin,0  ,	0.f,
				camsin, camcos,	0.f,	0.f,
				0.f, 0.f,		1.f,	0.f,
				0.f, 0.f,		0.f,	1.f
			};
			ViewMatrix *= RotZ;
			UpdateViewMatrix();
		}
	}
	else
	{
		if (m_FrontRotation + Dir.z <= m_maxFrontRotation)
		{
			m_FrontRotation += Dir.z;
			glm::mat4 RotZ
			{
				camcos, -camsin,0  ,	0.f,
				camsin, camcos,	0.f,	0.f,
				0.f, 0.f,		1.f,	0.f,
				0.f, 0.f,		0.f,	1.f
			};
			ViewMatrix *= RotZ;
			UpdateViewMatrix();
		}
	}
}

void CMyCameraFP::rotateRight(glm::vec3 Dir)
{
	float camcos = cosf(Dir.y / 100.f);
	float camsin = sinf(Dir.y / 100.f);

	if (Dir.y < 0)
	{
		if (m_RightRotation + Dir.y >= -m_maxRightRotation)
		{
			m_RightRotation += Dir.y;
			glm::mat4 RotY
			{
				1.f, 0.f,	  0.f,	  0.f,
				0.f, camcos, -camsin, 0.f,
				0.f, camsin, camcos,  0.f,
				0.f, 0.f,	 0.f,	  1.f
			};
			ViewMatrix *= RotY;
			UpdateViewMatrix();
		}
	}
	else
	{
		if (m_RightRotation + Dir.y <= m_maxRightRotation)
		{
			m_RightRotation += Dir.y;
			glm::mat4 RotY
			{
				1.f, 0.f,	  0.f,	  0.f,
				0.f, camcos, -camsin, 0.f,
				0.f, camsin, camcos,  0.f,
				0.f, 0.f,	 0.f,	  1.f
			};
			ViewMatrix *= RotY;
			UpdateViewMatrix();
		}
	}
}