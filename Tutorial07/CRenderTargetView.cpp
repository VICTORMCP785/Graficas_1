#include "CRenderTargetView.h"

CRenderTargetView::CRenderTargetView()
{
}

void CRenderTargetView::Init(RenderTargetViewStruct RTVSi)
{
#ifdef D3D11
	ZeroMemory(&m_Desc, sizeof(m_Desc));
	m_Desc.Format = (DXGI_FORMAT)RTVSi.format;
	m_Desc.ViewDimension = (D3D11_RTV_DIMENSION)RTVSi.viewDimension;
	m_Desc.Texture2D.MipSlice = RTVSi.texture2D.mipSlice;
#endif
}

void CRenderTargetView::Update()
{
}

void CRenderTargetView::Destroy()
{
}

void CRenderTargetView::Render()
{
}
