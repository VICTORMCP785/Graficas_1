#include "CMyCamara.h"
#include <dinput.h>

CMyCamara::CMyCamara()
{

}

CMyCamara::~CMyCamara()
{

}

unsigned int CMyCamara::Init(MyStructCamera STR)
{
	//Inicializar las variables
	Set_Position(STR.Position);
	Set_LookAt(STR.LookAt);
	Set_up(STR.Up);
	Set_FOV(STR.FOV);
	Set_Width(STR.Width);
	Set_Height(STR.height);
	Set_Near(STR.Near);
	Set_Far(STR.Far);


	GeneratoViewMatrix();
	UpdateViewMatrix();
	UpdateProyection();

	return 0;
}

void CMyCamara::SetPos(glm::vec3 NewPos)
{
	F.Position = NewPos;
	UpdateViewMatrix();
}

void CMyCamara::UpdateProyection()
{
	Proyeccion = glm::perspectiveFovLH(F.FOV, F.Width, F.height, F.Near, F.Far);
	Proyeccion = glm::transpose(Proyeccion);
}

void CMyCamara::GeneratoViewMatrix()
{
	Set_Front();
	Set_Rigth();
	Set_Up();

	glm::mat4 Axis
	{
		F.Rigth.x,	F.Rigth.y,	F.Rigth.z,	0,
		UpRef.x,	UpRef.y,	UpRef.z,	0,
		F.Front.x,	F.Front.y,	F.Front.z,	0,
		0,          0,          0,          1
	};

	glm::mat4 Pos
	{
		1, 0, 0, -F.Position.x,
		0, 1, 0, -F.Position.y,
		0, 0, 1, -F.Position.z,
		0, 0, 0, 1
	};

	ViewMatrix = Pos * Axis;
}

void CMyCamara::UpdateViewMatrix()
{
	F.Rigth = { ViewMatrix[0][0], ViewMatrix[0][1], ViewMatrix[0][2] };
	UpRef	= { ViewMatrix[1][0], ViewMatrix[1][1], ViewMatrix[1][2] };
	F.Front = { ViewMatrix[2][0], ViewMatrix[2][1], ViewMatrix[2][2] };
	
	F.LookAt = F.Front + F.Position;
}


void CMyCamara::move()
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

	if (mUp)
	{
		F.Position += (UpRef * F.Movement);
	}
	if (mDown)
	{
		F.Position += (UpRef * -F.Movement);
	}

	glm::mat4 Axis
	{
		F.Rigth.x,	F.Rigth.y,	F.Rigth.z,	0,
		UpRef.x,	UpRef.y,	UpRef.z,	0,
		F.Front.x,	F.Front.y,	F.Front.z,	0,
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
//void CMyCamara::Rotate(glm::vec3 Dir)
//{
//	RotateHor(Dir);
//	RotateVer(Dir);
//}
//
//void CMyCamara::RotateHor(glm::vec3 Dire)
//{
//	//Horizontal
//	float CosRes = cosf(Dire.x / 100.0f);
//	float SenRes = sinf(Dire.x / 100.0f);
//	
//	glm::mat4 RotHor {
//		CosRes,  0.f, SenRes,    0.f,
//		0.f,     1.f, 0.f,       0.f,
//		-SenRes, 0.f, CosRes,    0.f,
//		0.f,     0.f, 0.f,       1.f
//	};
//
//	ViewMatrix = ViewMatrix * RotHor;
//	UpdateViewMatrix();
//	GeneratoViewMatrix();
//}
//
//void CMyCamara::RotateVer(glm::vec3 Dire)
//{
//	//Vertical
//	float CosRes = cosf(Dire.y / 100.0f);
//	float SenRes = sinf(Dire.y / 100.0f);
//
//	glm::mat4 RotVer {
//		1.f, 0.f,    0.f,	    0.f,
//		0.f, CosRes, -SenRes,	0.f,
//		0.f, SenRes, CosRes,	0.f,
//		0.f, 0.f,    0.f,       1.f
//	};
//
//	ViewMatrix = ViewMatrix * RotVer;
//	UpdateViewMatrix();
//	GeneratoViewMatrix();
//}

void CMyCamara::rotate()
{
	if (mRotateLeft)
	{
		rotateFront({ 0.f, 0.f, F.Rotation });
	}
	if (mRotateRight)
	{
		rotateFront({ 0.f, 0.f, -F.Rotation });
	}
}

void CMyCamara::rotate(glm::vec3 mouseDir)
{
	rotateUp(mouseDir);
	rotateRight(mouseDir);
}

void CMyCamara::rotateUp(glm::vec3 Dir)
{
	float camcos = cosf(Dir.x / 100.f);
	float camsin = sinf(Dir.x / 100.f);

	glm::mat4 RotX
	{
		camcos,  0.f, camsin,	0.f,
		0.f,	 1.f, 0.f,		0.f,
		-camsin, 0.f, camcos,	0.f,
		0.f,	 0.f, 0.f,		1.f
	};
	ViewMatrix *= RotX;
	UpdateViewMatrix();
}

void CMyCamara::rotateRight(glm::vec3 Dir)
{
	float camcos = cosf(Dir.y / 100.f);
	float camsin = sinf(Dir.y / 100.f);

	glm::mat4 RotY
	{
		1.f, 0.f,	  0.f,	  0.f,
		0.f, camcos, -camsin, 0.f,
		0.f, camsin, camcos,  0.f,
		0.f, 0.f,	 0.f,	  1.f
	};
	ViewMatrix *= RotY;
	UpdateViewMatrix();
	//YRot=
}

void CMyCamara::rotateFront(glm::vec3 Dir)
{
	float camcos = cosf(Dir.z / 100.f);
	float camsin = sinf(Dir.z / 100.f);

	glm::mat4 RotZ
	{
		camcos, -camsin,	0.0f,	0.0f,
		camsin, camcos,		0.0f,	0.0f,
		0.0f,	0.0f,		1.0f,	0.0f,
		0.0f,	0.0f,		0.0f,	1.0f
	};
	ViewMatrix *= RotZ;
	UpdateViewMatrix();
}

void CMyCamara::getKeyPress(WPARAM key)
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
	//Y Movement
	if (key == 'Q')
	{
		mUp = true;
	}
	else if (key == 'E')
	{
		mDown = true;
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

void CMyCamara::getKeyRelease(WPARAM key)
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
	if (key == 'Q')
	{
		mUp = false;
	}
	else if (key == 'E')
	{
		mDown = false;
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


void CMyCamara::Set_up(glm::vec3 NewVect)
{
	UpRef = NewVect;
}

void CMyCamara::Set_LookAt(glm::vec3 NewVect)
{
	F.LookAt = NewVect;
}

void CMyCamara::Set_Position(glm::vec3 NewVect)
{
	F.Position = NewVect;
}

void CMyCamara::Set_Front()
{
	F.Front = (F.LookAt - F.Position);
	F.Front = glm::normalize(F.Front);
}

void CMyCamara::Set_Rigth()
{
	F.Rigth = glm::cross(UpRef, F.Front);
	F.Rigth = glm::normalize(F.Rigth);
}

void CMyCamara::Set_Up()
{
	F.Up = glm::cross(F.Front, F.Rigth);
	F.Up = glm::normalize(F.Up);
}

void CMyCamara::Set_Width(float NewWidth)
{
	F.Width = NewWidth;
}

void CMyCamara::Set_Height(float Newheight)
{
	F.height = Newheight;
}

void CMyCamara::Set_Near(float NewNear)
{
	F.Near = NewNear;
}

void CMyCamara::Set_Far(float NewFar)
{
	F.Far = NewFar;
}

void CMyCamara::Set_FOV(float NewFOV)
{
	F.FOV = NewFOV;
}

glm::vec3 CMyCamara::GetUp()
{
	return F.Up;
}

glm::vec3 CMyCamara::GetFront()
{
	return F.Front;
}

glm::vec3 CMyCamara::GetRigth()
{
	return F.Rigth;
}

glm::vec3 CMyCamara::GetLookAt()
{
	return F.LookAt;
}

glm::vec3 CMyCamara::GetPosition()
{
	return F.Position;
}

float CMyCamara::GetWidth()
{
	return F.Width;
}

float CMyCamara::GetHeight()
{
	return F.height;
}

float CMyCamara::GetNear()
{
	return F.Near;
}

float CMyCamara::GetFar()
{
	return F.Far;
}

float CMyCamara::GetFOV()
{
	return F.FOV;
}

glm::mat4 CMyCamara::GetViewMatrix()
{
	return ViewMatrix;
}

glm::mat4 CMyCamara::GetProyectionMatrix()
{
	return Proyeccion;
}

