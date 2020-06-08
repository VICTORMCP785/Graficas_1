#include "CDevice.h"


CDevice::CDevice()
{

}

//CDevice * CDevice::getDevice()
//{
//	if (D == nullptr)
//	{
//		D = new CDevice();
//	}
//	return D;
//}

void CDevice::Init(DeviceStruct DS)
{
	m_DS = DS;
}

void CDevice::Update()
{

}

void CDevice::Destroy()
{

}

void CDevice::Render()
{

}

CRenderTarget CDevice::CreateDepthStencilTexture(float width, float height)
{
#ifdef D3D11
	RenderTargetStruct descDepth;
	CRenderTarget DST;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.W = width;
	descDepth.H = height;
	descDepth.mipLevels = 1;
	descDepth.arraySize = 1;
	descDepth.format = FORMAT_D24_UNORM_S8_UINT;
	descDepth.sampleDesc.count = 1;
	descDepth.sampleDesc.quality = 0;
	descDepth.usage = USAGE_DEFAULT;
	descDepth.flags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.cpuAccessFlags = 0;
	descDepth.miscFlags = 0;
	
	DST.Init(descDepth);
	HR = m_DeviceD11->CreateTexture2D(&DST.Texture2DDesc, NULL, &DST.Texture2D);
	return DST;
#endif
}

void CDevice::CreateDepthStencilView(CRenderTarget depthstencil)
{
	

}
