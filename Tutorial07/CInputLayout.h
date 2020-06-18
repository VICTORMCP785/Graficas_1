#pragma once
#include "Snake.h"
struct InputLayoutStruct
{
	ID3DBlob * pShaderBlob;
	ID3D11Device * pD3DDevice; 
};
class CInputLayout
{
private:

public:
	CInputLayout();
	InputLayoutStruct ILS;
#ifdef D3D11
	ID3D11InputLayout ** InputLayout = NULL;
#endif
	void Init(InputLayoutStruct);
	HRESULT CreateInputLayoutDescFromVertexShaderSignature();
	void Update();
	void Destroy();
	void Render();
};

