#include "CRenderTarget.h"

CRenderTarget::CRenderTarget()
{
}

void CRenderTarget::Init(RenderTargetStruct S)
{
	RTS = S;
#ifdef D3D11
	ZeroMemory(&Texture2DDesc, sizeof(Texture2DDesc));

	Texture2DDesc.Width = RTS.W;
	Texture2DDesc.Height = RTS.H;
	Texture2DDesc.MipLevels = RTS.mipLevels;
	Texture2DDesc.ArraySize = RTS.arraySize;
	Texture2DDesc.Format = (DXGI_FORMAT)RTS.format;
	Texture2DDesc.SampleDesc.Count = RTS.sampleDesc.count;
	Texture2DDesc.SampleDesc.Quality = RTS.sampleDesc.quality;
	Texture2DDesc.Usage = (D3D11_USAGE)RTS.usage;
	Texture2DDesc.BindFlags = RTS.flags;
	Texture2DDesc.CPUAccessFlags = RTS.cpuAccessFlags;
	Texture2DDesc.MiscFlags = RTS.miscFlags;
#endif
}

void CRenderTarget::Update()
{
}

void CRenderTarget::Destroy()
{
}

void CRenderTarget::Render()
{
}
