#pragma once
#include "Snake.h"
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
};

