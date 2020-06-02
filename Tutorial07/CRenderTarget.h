#pragma once
#include "Snake.h"
struct RenderTargetStruct
{
#ifdef D3D11
	unsigned int W;
	unsigned int H;
	unsigned int mipLevels;
	unsigned int arraySize;
	FORMAT format;
	SAMPLEDESC sampleDesc;
	USAGE usage;
	unsigned int flags;
	unsigned int cpuAccessFlags;
	unsigned int miscFlags;
#endif
};
class CRenderTarget
{
private:

public:
	CRenderTarget();
	RenderTargetStruct RTS;
#ifdef D3D11
	ID3D11Texture2D * Texture2D = NULL;
	D3D11_TEXTURE2D_DESC Texture2DDesc;
#endif
	void Init(RenderTargetStruct RTSi);
	void Update();
	void Destroy();
	void Render();
};

//Clase Textura 
//rendertarget
//rendertargetview
