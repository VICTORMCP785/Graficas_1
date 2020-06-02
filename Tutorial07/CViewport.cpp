#include "CViewport.h"

CViewport::CViewport()
{
}

void CViewport::Init( ViewportStruct VPSi )
{
	VPS = VPSi;
#ifdef D3D11
	Viewport.Width = VPSi.Width;
	Viewport.Height = VPSi.Height;
	Viewport.MinDepth = VPSi.MinDepth;
	Viewport.MaxDepth = VPSi.MaxDepth;
	Viewport.TopLeftX = VPSi.TopLeftX;
	Viewport.TopLeftY = VPSi.TopLeftY;	
#endif
}

void CViewport::Update()
{
}

void CViewport::Destroy()
{
}

void CViewport::Render()
{
}
