#pragma once
#include "Snake.h"

struct SwapChainStruct
{
	unsigned int bufferCount;
	unsigned int W;
	unsigned int H;
	FORMAT format;
	unsigned int refreshNumerator;
	unsigned int refreshDenominator;
	unsigned int bufferUsage;
	unsigned int outputWND;
	unsigned int count;
	unsigned int quality;
	bool windowed;
};
class CSwapChain
{
private:
	static CSwapChain * SC;
	CSwapChain();
public:
	CSwapChain(const CSwapChain&) = delete;
	CSwapChain& operator=(const CSwapChain & L) = delete;
	static CSwapChain * getSwapChain();
	SwapChainStruct SCS;
#ifdef D3D11
	IDXGISwapChain * DXSC = NULL;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
#endif
	void Init(SwapChainStruct SCSi);
	void Update();
	void Destroy();
	void Render();
};

