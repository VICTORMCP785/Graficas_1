#pragma once
#include "Snake.h"
#include "CViewport.h"
#include "CVertexShader.h"
#include "Buffer.h"
#include "CMesh.h"
class CDeviceContext
{
private:
	//static CDeviceContext * DC;
	

public:
	/*CDeviceContext(const CDeviceContext&) = delete;
	CDeviceContext& operator=(const CDeviceContext & L) = delete;
	static CDeviceContext * getDeviceContext();*/

	CDeviceContext();
	~CDeviceContext();
	
#ifdef D3D11
	ID3D11DeviceContext* m_DeviceContext = NULL;
#endif
	void Init();
	void Update();
	void Destroy();
	void Render();

	CViewport setViwePort(float width, float height);
	void SetInputLayout(CVertexShader Vs);
	void SetVertexBuffer(CBuffer VB);
	void SetIndexBuffer(CBuffer IB);
	void SetprimitiveTopology();
};

