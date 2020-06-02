#include "CVertexShader.h"

void * CVertexShader::getBlob()
{
	return nullptr;
}

void * CVertexShader::getVS()
{
	return nullptr;
}

void * CVertexShader::getInputLayout()
{
	return nullptr;
}

CVertexShader::CVertexShader()
{
}

void CVertexShader::Init(VertexShaderStruct VSSi)
{
	VSS = VSSi;
}

void CVertexShader::Update()
{
}

void CVertexShader::Destroy()
{
}

void CVertexShader::Render()
{
}
