#pragma once
#include "CDevice.h"
#include "CDeviceContext.h"
#include "CSwapChain.h"
#include <vector>
#include <iostream>
#include <sstream>
#include "CMyCamara.h"
#include "CMyCameraFP.h"
#include "CRenderTargetView.h"

//#include "CDepthStencilView.h"
#include "CPixelShader.h"
#include "CInputLayout.h"
#include "CSamplerState.h"

class CApiManagerDX11
{
private:
	CApiManagerDX11();
	static CApiManagerDX11 *ApiManagerDX11;
public:
	~CApiManagerDX11();
	CApiManagerDX11(const CApiManagerDX11&) = delete;
	CApiManagerDX11& operator=(const CApiManagerDX11 & L) = delete;
	static CApiManagerDX11 * getApiManagerDX11();

	void initDevice();
	void initSwapChain(float width, float height, HWND hWnd);
	void initDeviceContext();
	void CreateRenderTargetView();
	void CreateDepthStencilTexture(float width, float height);
	void CreateDepthStencilView();
	void SetViewPort(float width, float height);
	void CreateVertexShader();
	void CreateInputLayout();
	void SetInputLayout();
	void CreatePixelShader();

	HRESULT HR;

	CDevice m_Device;
	CDeviceContext m_DeviceContext;
	CSwapChain m_SwapChain;

	CRenderTarget m_DepthStencil;
	CDepthStencilView m_DepthStencilView;
	CViewport m_ViewPort;
	CVertexShader m_VertexShader;
	CPixelShader m_PixelShader;

};

