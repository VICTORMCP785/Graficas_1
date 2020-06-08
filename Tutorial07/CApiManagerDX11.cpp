#include "CApiManagerDX11.h"

CApiManagerDX11::CApiManagerDX11()
{
}

CApiManagerDX11::~CApiManagerDX11()
{
}

CApiManagerDX11 * CApiManagerDX11::getApiManagerDX11()
{
	if (ApiManagerDX11 == nullptr)
	{
		ApiManagerDX11 = new CApiManagerDX11();
	}
	return ApiManagerDX11;
}

void CApiManagerDX11::initDdevice()
{
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	DeviceStruct DSiXL;
#ifdef D3D11
	DSiXL.Dev_Flag = createDeviceFlags;
	DSiXL.DriverTypeArr[0] = D3D_DRIVER_TYPE_HARDWARE;
	DSiXL.DriverTypeArr[1] = D3D_DRIVER_TYPE_WARP;
	DSiXL.DriverTypeArr[2] = D3D_DRIVER_TYPE_REFERENCE;
	DSiXL.FeatureLevel[0] = D3D_FEATURE_LEVEL_11_0;
	DSiXL.FeatureLevel[1] = D3D_FEATURE_LEVEL_10_1;
	DSiXL.FeatureLevel[2] = D3D_FEATURE_LEVEL_10_0;
	DSiXL.numFeatureLevel = ARRAYSIZE(DSiXL.FeatureLevel);

	m_Device.Init(DSiXL);
#endif
}

void CApiManagerDX11::CreateRenderTargetView()
{

}

void CApiManagerDX11::CreateDepthStencilTexture(float width, float height)
{
	m_DepthStencil = m_Device.CreateDepthStencilTexture(width, height);
}

void CApiManagerDX11::CreateDepthStencilView()
{

}

