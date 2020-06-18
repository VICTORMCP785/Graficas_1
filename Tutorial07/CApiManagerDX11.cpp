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

void CApiManagerDX11::GetBuffer()
{
	m_BackBuffer = m_SwapChain.getbuffer();
}

void CApiManagerDX11::CreateRenderTargetView()
{
	m_RendertargetView = m_Device.CreateRenderTargetView(m_BackBuffer);
	m_BackBuffer.Texture2D->Release();
}

void CApiManagerDX11::CreateDepthStencilTexture(float width, float height)
{
	m_DepthStencil = m_Device.CreateDepthStencilTexture(width, height);
}

void CApiManagerDX11::CreateDepthStencilView()
{
	m_DepthStencilView = m_Device.CreateDepthStencilView(m_DepthStencil);
}

void CApiManagerDX11::SetViewPort(float width, float height)
{
	m_ViewPort = m_DeviceContext.setViwePort(width, height);
}

void CApiManagerDX11::CreateVertexShader()
{
	m_VertexShader = m_Device.CreateVertexShader();
}

void CApiManagerDX11::CreateInputLayout()
{
	m_Device.CreateInputLayoutDescFromVertexShaderSignature(m_VertexShader.m_pVSBlob, m_Device.m_DeviceD11, &m_VertexShader.m_pInputLayout);
}

void CApiManagerDX11::SetInputLayout()
{
	m_DeviceContext.SetInputLayout(m_VertexShader);
}

void CApiManagerDX11::CreatePixelShader()
{
	m_PixelShader = m_Device.CreatePixelShader();
}

void CApiManagerDX11::CreateVertexbuffer()
{
	m_VertexBuffer = m_Device.CreateVertexBuffer();
}

void CApiManagerDX11::SetVertexBuffer()
{
	m_DeviceContext.SetVertexBuffer(m_VertexBuffer);
}

void CApiManagerDX11::CreateIndexBuffer()
{
	m_IndexBuffer = m_Device.CreateIndexBuffer();
}

void CApiManagerDX11::SetIndexBuffer()
{
	m_DeviceContext.SetIndexBuffer(m_IndexBuffer);
}

void CApiManagerDX11::SetPrimitiveTopology()
{
	m_DeviceContext.SetprimitiveTopology();
}

void CApiManagerDX11::CreateSamplerState()
{
	m_SamplerLinear = m_Device.CreateSamplerState();
}

