#include "CDeviceContext.h"
#include "CDevice.h"
#include "CMesh.h"

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
#ifdef D3D11
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
#endif
}

void CDeviceContext::SetInputLayout(CVertexShader Vs)
{
#ifdef D3D11
	m_DeviceContext->IASetInputLayout(Vs.m_pInputLayout);
#endif
}

void CDeviceContext::SetVertexBuffer(CBuffer mesh)
{
#ifdef D3D11
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &mesh.VertexBufferD11, &stride, &offset);
#endif
}

void CDeviceContext::SetIndexBuffer(CBuffer IB)
{
#ifdef D3D11
	m_DeviceContext->IASetIndexBuffer(IB.IndexBufferD11, DXGI_FORMAT_R16_UINT, 0);
#endif
}

void CDeviceContext::SetprimitiveTopology()
{
#ifdef D3D11
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#endif
}
