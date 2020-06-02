#pragma once
#include "Snake.h"
struct VertexShaderStruct
{

};
class CVertexShader
{
private:

public:
	void * getBlob();
	void * getVS();
	void * getInputLayout();
	CVertexShader();
	VertexShaderStruct VSS;
#ifdef D3D11
	ID3D11VertexShader * VertexShader = NULL;
	ID3D11InputLayout * m_pInputLayout = NULL;
	ID3DBlob * m_pVSBlob = NULL;
#endif
	void Init(VertexShaderStruct);
	void Update();
	void Destroy();
	void Render();
};

