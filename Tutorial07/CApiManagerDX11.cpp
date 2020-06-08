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

void CApiManagerDX11::initDevice()
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

void CApiManagerDX11::initSwapChain(float width, float height, HWND hWnd)
{
	SwapChainStruct sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.bufferCount = 1;
	sd.W = width;
	sd.H = height;
	sd.format = FORMAT_R8G8B8A8_UNORM;
	sd.refreshNumerator = 60;
	sd.refreshDenominator = 1;
	sd.bufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.outputWND = (unsigned int)hWnd;
	sd.count = 1;
	sd.quality = 0;
	sd.windowed = TRUE;
	m_SwapChain.Init(sd);
}

void CApiManagerDX11::initDeviceContext()
{
	UINT numDriverTypes = ARRAYSIZE(m_Device.m_DS.DriverTypeArr);
	FEATURE_LEVEL featureLevel = FEATURE_LEVEL_11_0;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_Device.m_DS.DriverType = m_Device.m_DS.DriverTypeArr[driverTypeIndex];
		HR = D3D11CreateDeviceAndSwapChain(NULL, (D3D_DRIVER_TYPE)m_Device.m_DS.DriverType, NULL,
			m_Device.m_DS.Dev_Flag, m_Device.m_DS.FeatureLevel,
			m_Device.m_DS.numFeatureLevel, D3D11_SDK_VERSION, &m_SwapChain.SwapChainDesc,
			&m_SwapChain.DXSC, &m_Device.m_DeviceD11, (D3D_FEATURE_LEVEL*)&featureLevel,
			&m_DeviceContext.m_DeviceContext);

		if (SUCCEEDED(HR))
			break;
	}
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

