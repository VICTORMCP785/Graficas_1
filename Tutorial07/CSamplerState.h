#pragma once
#include "Snake.h"
struct SamplerStateStruct
{
	FILTER filter;
	TEXTURE_ADDRESS_MODE addresU;
	TEXTURE_ADDRESS_MODE addresV;
	TEXTURE_ADDRESS_MODE addresW;
	float mipLODBias;
	unsigned int maxAnisotropy;
	COMPARISON_FUNC comparisonFunc;
	float borderColor[4];
	float minLOD;
	float maxLOD;
};
class CSamplerState
{
private:

public:
	CSamplerState();
	SamplerStateStruct SSS;
#ifdef D3D11
	ID3D11SamplerState * SamplerStates = NULL;
	D3D11_SAMPLER_DESC Desc;
#endif
	void Init(SamplerStateStruct SSSi);
	void Update();
	void Destroy();
	void Render();
};

