#include "CDeviceContext.h"

CDeviceContext::CDeviceContext()
{
}

CDeviceContext::~CDeviceContext()
{

}

//CDeviceContext * CDeviceContext::getDeviceContext()
//{
//	/*if (DC == nullptr)
//	{
//		DC = new CDeviceContext();
//	}
//	return DC;*/
//}

void CDeviceContext::Init()
{
}

void CDeviceContext::Update()
{
}

void CDeviceContext::Destroy()
{
}

void CDeviceContext::Render()
{
}

CViewport CDeviceContext::setViwePort(float width, float height)
{
	CViewport VP;
	ViewportStruct vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	VP.Init(vp);

	m_DeviceContext->RSSetViewports(1, &VP.Viewport);

	return VP;
}
