#pragma once
#include "Snake.h"
struct ViewportStruct
{
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;
	int TopLeftX;
	int TopLeftY;
};
class CViewport
{
private:

public:
	CViewport();
	ViewportStruct VPS;
#ifdef D3D11
	D3D11_VIEWPORT Viewport;
#endif
	void Init( ViewportStruct VPSi );
	void Update();
	void Destroy();
	void Render();
};

