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
#include "CVertexShader.h"
#include "CDepthStencilView.h"
#include "CPixelShader.h"
#include "CInputLayout.h"
#include "CSamplerState.h"
#include "CViewport.h"

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

	void initDdevice();
	void CreateRenderTargetView();
	void CreateDepthStencilTexture(float width, float height);
	void CreateDepthStencilView();

	HRESULT HR;
	CDevice m_Device;
	CRenderTarget m_DepthStencil;
	CDepthStencilView m_DepthStencilView;
	
};

