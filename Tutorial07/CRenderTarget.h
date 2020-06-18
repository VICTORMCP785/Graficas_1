#pragma once
#include "Snake.h"
struct Texture2DStruct
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
class CTexture2D
{
private:

public:
	CTexture2D();
	Texture2DStruct RTS;
#ifdef D3D11
	D3D11_TEXTURE2D_DESC Texture2DDesc;
	ID3D11Texture2D * Texture2D = NULL;
#endif
	void Init(Texture2DStruct RTSi);
	void Update();
	void Destroy();
	void Render();
};

//Clase Textura 
//rendertarget
//rendertargetview
