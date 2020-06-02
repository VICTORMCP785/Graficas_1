#pragma once
#include "Snake.h"

class CPixelShader
{
private:

public:
	CPixelShader();
#ifdef D3D11
	ID3D11PixelShader * PixelShader = NULL;
	ID3DBlob* pPSBlob = NULL;
#endif
	void Init();
	void Update();
	void Destroy();
	void Render();
};

