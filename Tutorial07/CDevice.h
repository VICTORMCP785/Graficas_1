#pragma once
#include "Snake.h"
#include "resource.h"
#include "CRenderTarget.h"
#include "CRenderTargetView.h"
#include "CDepthStencilView.h"
#include "CVertexShader.h"
#include "CPixelShader.h"
#include "CInputLayout.h"
#include "Buffer.h"
#include "CSamplerState.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"

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

struct SimpleVertex
{
	glm::vec3 Pos;
	glm::vec2 Tex;
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

	CRenderTargetView CreateRenderTargetView(CTexture2D bb);
	CTexture2D CreateDepthStencilTexture( float width, float height );
	CDepthStencilView CreateDepthStencilView(CTexture2D depthstencil );
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	CVertexShader CreateVertexShader();
	CInputLayout CreateInputLayoutDesc(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);
	CPixelShader CreatePixelShader();
	CBuffer CreateVertexBuffer();
	CBuffer CreateIndexBuffer();
	CSamplerState CreateSamplerState();

	HRESULT HR;

	
};

