#include "CDepthStencilView.h"

CDepthStencilView::CDepthStencilView()
{
}

void * CDepthStencilView::getDSV()
{
#ifdef D3D11
	return DepthStencilView;
#else
	return nullptr;
#endif
}

void CDepthStencilView::Init(DepthStencilViewStruct DSVSi , FORMAT F)
{
#ifdef D3D11
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = (DXGI_FORMAT)F;
	descDSV.ViewDimension = (D3D11_DSV_DIMENSION)DSVSi.viewDimension;
	descDSV.Texture2D.MipSlice = DSVSi.texture2D.mipSlice;
#endif // D3D11
}

void CDepthStencilView::Update()
{
}

void CDepthStencilView::Destroy()
{
}

void CDepthStencilView::Render()
{
}
