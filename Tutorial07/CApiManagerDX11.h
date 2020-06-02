#pragma once
#include "CDevice.h"
#include "CDeviceContext.h"
#include "CSwapChain.h"
#include "resource.h"

class CApiManagerDX11
{
private:
	CApiManagerDX11();
	static CApiManagerDX11 *ApiManagerDX11;
public:
	~CApiManagerDX11();
	CApiManagerDX11(const CApiManagerDX11&) = delete;
	CApiManagerDX11& operator=(const CApiManagerDX11 & L) = delete;
	static CApiManagerDX11 * getApiManagerDX11();

	void initDdevice();

	CDevice m_Device;
};

