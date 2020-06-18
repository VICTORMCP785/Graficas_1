#include "CDeviceContext.h"
#include "CDevice.h"

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

void CDeviceContext::SetInputLayout(CVertexShader Vs)
{
	m_DeviceContext->IASetInputLayout(Vs.m_pInputLayout);
}

void CDeviceContext::SetVertexBuffer(CBuffer VB)
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &VB.VertexBufferD11, &stride, &offset);
}

void CDeviceContext::SetIndexBuffer(CBuffer IB)
{
	m_DeviceContext->IASetIndexBuffer(IB.IndexBufferD11, DXGI_FORMAT_R16_UINT, 0);
}

void CDeviceContext::SetprimitiveTopology()
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
