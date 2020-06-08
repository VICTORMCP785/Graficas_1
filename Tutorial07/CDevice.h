#pragma once
#include "Snake.h"
#include "resource.h"
#include "CRenderTarget.h"
#include "CDepthStencilView.h"

struct DeviceStruct
{
#ifdef D3D11
	D3D_DRIVER_TYPE DriverTypeArr[3];
	D3D_DRIVER_TYPE DriverType;
	UINT Dev_Flag;
	D3D_FEATURE_LEVEL FeatureLevel[3];
	UINT numFeatureLevel;
#endif
};

class CDevice
{
private:
	//static CDevice * D;
	
	
public:
	CDevice();
	//CDevice(const CDevice&) = delete;
	//CDevice& operator=(const CDevice & L) = delete;
	//static CDevice * getDevice();
	DeviceStruct m_DS;
#ifdef D3D11
	ID3D11Device * m_DeviceD11 = NULL;
#endif
	void Init(DeviceStruct DS);
	void Update();
	void Destroy();
	void Render();

	CRenderTarget CreateDepthStencilTexture( float width, float height );
	void CreateDepthStencilView( CRenderTarget depthstencil );

	HRESULT HR;

	/*
	void CreateRenderTargetView();
	void CreateDepthStencilTexture();
	void CreateDepthStencilView();
	void CreaateVertexShader();
	void CreateInputLayout();
	void CreatePixelShader();
	void CreateVertexBuffer();*/
};

