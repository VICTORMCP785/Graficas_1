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
	void GetBuffer();
	void CreateRenderTargetView();
	void CreateDepthStencilTexture(float width, float height);
	void CreateDepthStencilView();
	void SetViewPort(float width, float height);
	void CreateVertexShader();
	void CreateInputLayout();
	void SetInputLayout();
	void CreatePixelShader();
	void CreateVertexbuffer();
	void SetVertexBuffer();
	void CreateIndexBuffer();
	void SetIndexBuffer();
	void SetPrimitiveTopology();
	void CreateSamplerState();

	HRESULT HR;

	CDevice m_Device;
	CDeviceContext m_DeviceContext;
	CSwapChain m_SwapChain;
	CRenderTargetView m_RendertargetView;
	CTexture2D m_DepthStencil;
	CDepthStencilView m_DepthStencilView;
	CViewport m_ViewPort;
	CVertexShader m_VertexShader;
	CPixelShader m_PixelShader;
	CTexture2D m_BackBuffer;
	CBuffer m_VertexBuffer;
	CBuffer m_IndexBuffer;
	CSamplerState m_SamplerLinear;
	CInputLayout m_inputlayout;
	
	//CBuffer m_BackBuffer;

};

