#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include "resource.h"
#include "Buffer.h"

struct MyStructCamera
{
	glm::vec3 Position;//set Get
	glm::vec3 LookAt;//set Get
	glm::vec3 Front;//set Get
	glm::vec3 Up;//set Set GetCalculado
	glm::vec3 Rigth;//set Get

	float Width;//Set Get
	float height;//Set Get
	float Near;//Set Get
	float Far;//Set Get
	float FOV;//Set Get

	const float Movement = 0.025f;
	const float Rotation = 0.25f;
};

class CMyCamara
{
public:

	glm::vec3 UpRef;
	MyStructCamera F;

	glm::mat4 ViewMatrix;
	glm::mat4 Proyeccion;


	CMyCamara();
	~CMyCamara();

	glm::vec3 PosFinal;
	glm::vec3 PosInicial;
	glm::vec3 Direccion;

	bool SiHalo;
	bool mIsClicked = false;
	bool mForward = false;
	bool mBack = false;
	bool mLeft = false;
	bool mRight = false;
	bool mUp = false;
	bool mDown = false;
	bool mRotateLeft = false;
	bool mRotateRight = false;
	float XRot;
	float YRot;
	float ZRot;

	void UpdateProyection();
	void GeneratoViewMatrix();
	void UpdateViewMatrix();
	unsigned int Init(MyStructCamera);
	void SetPos(glm::vec3);
	
	virtual void move();
	void rotate();
	void rotate(glm::vec3 mouseDir);
	void rotateUp(glm::vec3 Dir);
	virtual void rotateRight(glm::vec3 Dir);
	virtual void rotateFront(glm::vec3 Dir);
	void getKeyPress(WPARAM key);
	void getKeyRelease(WPARAM key);


	//Set
	void Set_up(glm::vec3);
	void Set_LookAt(glm::vec3);
	void Set_Position(glm::vec3);
	void Set_Front();
	void Set_Rigth();
	void Set_Up();
	void Set_Width(float);
	void Set_Height(float);
	void Set_Near(float);
	void Set_Far(float);
	void Set_FOV(float);

	//Get
	glm::vec3 GetUp();
	glm::vec3 GetFront();
	glm::vec3 GetRigth();
	glm::vec3 GetLookAt();
	glm::vec3 GetPosition();
	float GetWidth();
	float GetHeight();
	float GetNear();
	float GetFar();
	float GetFOV();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProyectionMatrix();

	CBuffer m_CBNeverChanges;
	CBuffer m_CBChangesEveryFrame;
	CBuffer m_CBChangesOnResize;
};

