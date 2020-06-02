#pragma once
#include "Snake.h"
struct RenderTargetViewStruct
{
	FORMAT format;
	RTV_DIMENSION viewDimension;
	union
	{
		BUFFER_RTV buffer;
		TEX1D_RTV texture1D;
		TEX1D_ARRAY_RTV texture1DArray;
		TEX2D_RTV texture2D;
		TEX2D_ARRAY_RTV texture2DArray;
		TEX2DMS_RTV texture2DMS;
		TEX2DMS_ARRAY_RTV texture2DMSArray;
		TEX3D_RTV texture3D;
	};
};

class CRenderTargetView
{
private:

public:
	CRenderTargetView();
	RenderTargetViewStruct RTVS;
#ifdef D3D11
	ID3D11RenderTargetView * RenderTargetView = NULL;
	D3D11_RENDER_TARGET_VIEW_DESC m_Desc;
#endif
	void Init(RenderTargetViewStruct RTVSi);
	void Update();
	void Destroy();
	void Render();
};

