#include "CSwapChain.h"

CSwapChain::CSwapChain()
{

}

CSwapChain * CSwapChain::getSwapChain()
{
	if (SC == nullptr)
	{
		SC = new CSwapChain();
	}
	return SC;
}

void CSwapChain::Init(SwapChainStruct SCSi)
{
#ifdef D3D11
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = SCSi.bufferCount;
	SwapChainDesc.BufferDesc.Width = SCSi.W;
	SwapChainDesc.BufferDesc.Height = SCSi.H;
	SwapChainDesc.BufferDesc.Format = (DXGI_FORMAT)SCSi.format;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = SCSi.refreshNumerator;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = SCSi.refreshDenominator;
	SwapChainDesc.BufferUsage = SCSi.bufferUsage;
	SwapChainDesc.OutputWindow = (HWND)SCSi.outputWND;
	SwapChainDesc.SampleDesc.Count = SCSi.count;
	SwapChainDesc.SampleDesc.Quality = SCSi.quality;
	SwapChainDesc.Windowed = SCSi.windowed;
#endif
}

void CSwapChain::Update()
{

}

void CSwapChain::Destroy()
{

}

void CSwapChain::Render()
{

}
