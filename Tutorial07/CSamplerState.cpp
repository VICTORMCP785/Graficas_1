#include "CSamplerState.h"

CSamplerState::CSamplerState()
{
}

void CSamplerState::Init(SamplerStateStruct S)
{
#ifdef D3D11
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.Filter = (D3D11_FILTER)S.filter;
	Desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)S.addresU;
	Desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)S.addresV;
	Desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)S.addresW;
	Desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)S.comparisonFunc;
	Desc.MinLOD = S.minLOD;
	Desc.MaxLOD = S.maxLOD;
#endif
}

void CSamplerState::Update()
{
}

void CSamplerState::Destroy()
{
}

void CSamplerState::Render()
{
}
