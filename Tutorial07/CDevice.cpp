#include "CDevice.h"
#include <vector>


CDevice::CDevice()
{

}

//CDevice * CDevice::getDevice()
//{
//	if (D == nullptr)
//	{
//		D = new CDevice();
//	}
//	return D;
//}

void CDevice::Init(DeviceStruct DS)
{
	m_DS = DS;
}

void CDevice::Update()
{

}

void CDevice::Destroy()
{

}

void CDevice::Render()
{

}

CRenderTargetView CDevice::CreateRenderTargetView(CTexture2D bb)
{
	CRenderTargetView RTV;
	HR = m_DeviceD11->CreateRenderTargetView(bb.Texture2D, NULL, &RTV.RenderTargetView);
	if (FAILED(HR))
	{

	}
	return RTV;
}

CTexture2D CDevice::CreateDepthStencilTexture(float width, float height)
{
#ifdef D3D11
	Texture2DStruct descDepth;
	CTexture2D DST;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.W = width;
	descDepth.H = height;
	descDepth.mipLevels = 1;
	descDepth.arraySize = 1;
	descDepth.format = FORMAT_D24_UNORM_S8_UINT;
	descDepth.sampleDesc.count = 1;
	descDepth.sampleDesc.quality = 0;
	descDepth.usage = USAGE_DEFAULT;
	descDepth.flags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.cpuAccessFlags = 0;
	descDepth.miscFlags = 0;
	
	DST.Init(descDepth);
	HR = m_DeviceD11->CreateTexture2D(&DST.Texture2DDesc, NULL, &DST.Texture2D);
	return DST;
#endif
}

CDepthStencilView CDevice::CreateDepthStencilView(CTexture2D depthstencil)
{
	DepthStencilViewStruct structDepthStencilView;
	CDepthStencilView DSV;
	ZeroMemory(&structDepthStencilView, sizeof(structDepthStencilView));
	structDepthStencilView.format = depthstencil.RTS.format;
	structDepthStencilView.viewDimension = DSV_DIMENSION_TEXTURE2D;
	structDepthStencilView.texture2D.mipSlice = 0;
	DSV.Init(structDepthStencilView, (FORMAT)depthstencil.Texture2DDesc.Format);

	HR = m_DeviceD11->CreateDepthStencilView(depthstencil.Texture2D, &DSV.descDSV, &DSV.DepthStencilView);
	if (FAILED(HR))
	{
		//F
	}
	return DSV;
}

HRESULT CDevice::CompileShaderFromFile(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

CVertexShader CDevice::CreateVertexShader()
{
	CVertexShader vs;
	HR = CompileShaderFromFile(L"Tutorial07.fx", "VS", "vs_4_0", &vs.m_pVSBlob);

	HR = m_DeviceD11->CreateVertexShader(vs.m_pVSBlob->GetBufferPointer(), vs.m_pVSBlob->GetBufferSize(), NULL, &vs.VertexShader);
	if (FAILED(HR))
	{
		vs.m_pVSBlob->Release();
		return vs;
	}
	return vs;
}

HRESULT CDevice::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob * pShaderBlob, ID3D11Device * pD3DDevice, ID3D11InputLayout ** pInputLayout)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector <D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	int offset = 0;
	for (int i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = offset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; offset += 12;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	return hr;
}

CPixelShader CDevice::CreatePixelShader()
{
	CPixelShader PS;
	HR = CompileShaderFromFile(L"Tutorial07.fx", "PS", "ps_4_0", &PS.pPSBlob);
	if (FAILED(HR))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
	}
	HR = m_DeviceD11->CreatePixelShader(PS.pPSBlob->GetBufferPointer(), PS.pPSBlob->GetBufferSize(), NULL, &PS.PixelShader);
	PS.pPSBlob->Release();
	return PS;
}

CBuffer CDevice::CreateVertexBuffer()
{
	SimpleVertex vertices[] =
	{
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
	};
	BufferStruct bd;//buffer parte del vertex
	CBuffer VB;
	ZeroMemory(&bd, sizeof(bd));
	bd.usage = USAGE_DEFAULT;
	bd.byteWidth = sizeof(SimpleVertex) * 24;
	bd.bindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.cpuAccessFlags = 0;
	SubresourceData subrsrcData;
	subrsrcData.psysMem = vertices;
	VB.Init(subrsrcData, bd);
	HR = m_DeviceD11->CreateBuffer(&VB.BufferDesc, &VB.SUBDATA, &VB.VertexBufferD11);
	return VB;
}

CBuffer CDevice::CreateIndexBuffer()
{
	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};
	BufferStruct bd;
	CBuffer IB;
	bd.usage = USAGE_DEFAULT;
	bd.byteWidth = sizeof(WORD) * 36;
	bd.bindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.cpuAccessFlags = 0;
	SubresourceData IDS;
	IDS.psysMem = indices;
	IB.Init(IDS, bd);

	HR = m_DeviceD11->CreateBuffer(&IB.BufferDesc, &IB.SUBDATA, &IB.IndexBufferD11);

	return IB;
}

CSamplerState CDevice::CreateSamplerState()
{
	SamplerStateStruct samplerDsc;
	CSamplerState SL;
	ZeroMemory(&samplerDsc, sizeof(samplerDsc));
	samplerDsc.filter = FILTER_MIN_MAG_MIP_LINEAR;
	samplerDsc.addresU = TEXTURE_ADDRESS_WRAP;
	samplerDsc.addresV = TEXTURE_ADDRESS_WRAP;
	samplerDsc.addresW = TEXTURE_ADDRESS_WRAP;
	samplerDsc.comparisonFunc = COMPARISON_NEVER;
	samplerDsc.minLOD = 0;
	samplerDsc.maxLOD = D3D11_FLOAT32_MAX;

	SL.Init(samplerDsc);

	m_DeviceD11->CreateSamplerState(&SL.Desc, &SL.SamplerStates);
	return SL;
}
