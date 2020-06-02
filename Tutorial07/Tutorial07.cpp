#include <vector>
#include <iostream>
#include <sstream>
#include "resource.h"
#include "CMyCamara.h"
#include "CMyCameraFP.h"
//#include "CDevice.h"
//#include "CDeviceContext.h"
//#include "CSwapChain.h"
//#include "VertexBuffer.h"
//#include "CIndexBuffer.h"
#include "CRenderTarget.h"
#include "CRenderTargetView.h"
#include "CVertexShader.h"
#include "CDepthStencilView.h"
#include "CPixelShader.h"
#include "CInputLayout.h"
#include "CSamplerState.h"
#include "CViewport.h"

#include "CApiManagerDX11.h"

#include "amgui/imgui.h"
#include "amgui/imgui_impl_dx11.h"
#include "amgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct SimpleVertex
{
    glm::vec3 Pos;
    glm::vec2 Tex;
};

struct CBNeverChanges
{
    glm::mat4 mView;
};

struct CBChangeOnResize
{
	glm::mat4 mProjection;
};

struct CBChangesEveryFrame
{
	glm::mat4 mWorld;
    glm::vec4 vMeshColor;
};

glm::vec3 Laberinto_Array[23]
{
	{ -2.f, 0.f, 10.f }, { 0.f, 0.f, 10.f },
	{ 2.f, 0.f, 10.f }, { 4.f, 0.f, 10.f },
	{ 6.f, 0.f, 10.f }, { 8.f, 0.f, 10.f },
	{ 10.f, 0.f, 10.f }, { 10.f, 0.f, 8.f },
	{ 10.f, 0.f, 6.f }, { 10.f, 0.f, 4.f },
	{ 8.f, 0.f, 4.f }, { 6.f, 0.f, 6.f },
	{ 6.f, 0.f, 4.f }, { 6.f, 0.f, 2.f },
	{ 4.f, 0.f, 2.f }, { 2.f, 0.f, 6.f },
	{ 0.f, 0.f, 6.f }, { -2.f, 0.f, 6.f },
	{ -2.f, 0.f, 4.f }, { 2.f, 0.f, 2.f },
	{ -2.f, 0.f, 2.f }, { -2.f, 0.f, 0.f },
	{ 2.f, 0.f, 0.f }
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
//CDevice * CDevice::D = nullptr;
CApiManagerDX11 * CApiManagerDX11::ApiManagerDX11 = nullptr;
CSwapChain * CSwapChain::SC = nullptr;
CDeviceContext * CDeviceContext::DC = nullptr;


HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;
#ifdef D3D11
ID3D11ShaderResourceView*           g_pTextureRV = NULL;
ID3D11ShaderResourceView*           g_TextureInactive = NULL;
#endif
glm::vec4                           g_MeshColor(0.7f, 0.7f, 0.7f, 1.0f);
CMyCamara							g_MyCamara;
CMyCameraFP							g_MyCamaraFP;
CRenderTarget						g_InteractiveCamaraTexure;
CDepthStencilView					g_DepthStencilViewFP;
CRenderTargetView					g_RenderTargedView2;
glm::mat4                           g_World;
FEATURE_LEVEL						g_featureLevel = FEATURE_LEVEL_11_0;
//CDevice*							g_Device;
CDeviceContext*						g_DeviceContext = CDeviceContext::getDeviceContext();
CSwapChain*							g_SwapChain = CSwapChain::getSwapChain();
DRIVER_TYPE							g_driverType = DRIVER_TYPE_NULL; 
glm::mat4                           g_View;
glm::mat4                           g_Projection;
CBuffer								g_VertexBuffer;
CBuffer								g_IndexBuffer;
CBuffer								g_NeverChange;
CBuffer								g_ChangeOnResize;
CBuffer								g_ChangeEveryFrame;
CRenderTarget						g_BackBuffer;
CRenderTarget						g_DepthStencil;
CRenderTargetView					g_RenderTargetView;
CDepthStencilView					g_DepthStencilView;
CVertexShader						g_VertexShader;
CPixelShader						g_PixelShader;
CInputLayout						g_InputLayout;
CSamplerState						g_SamplerState;
CViewport							g_Viewport;
CMyCamara *							ActiveCamara = NULL;
CMyCamara *							InactiveCamara = NULL;

CApiManagerDX11*					g_ApiManager = CApiManagerDX11::getApiManagerDX11();

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();

#ifdef D3D11
HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
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
#endif
//inputlaiout en el struct pedir un blop
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
#ifdef D3D11
			ImVec2 WindowDimension(240, 240);
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Change Cameras");

			if (ImGui::Button("Click me"))
			{
				CMyCamara * temp = InactiveCamara;
				InactiveCamara = ActiveCamara;
				ActiveCamara = temp;
			}
			ImGui::End();

			ImGui::Begin("DirectX11 Texture Test");
			ImGui::Image(g_TextureInactive, WindowDimension);
			float x, y, z;
			std::ostringstream ss;
			std::ostringstream sd;
			std::ostringstream sc;
			x = ActiveCamara->GetPosition().x;
			y = ActiveCamara->GetPosition().y;
			z = ActiveCamara->GetPosition().z;
			ss << x;
			std::string TX(ss.str());
			sd << y;
			std::string TY(sd.str());
			sc << z;
			std::string TZ(sc.str());
			std::string Traslacion = "X "+ TX + "  Y " + TY + "  Z " + TZ;
			ImGui::Text("Traslacion:");
			ImGui::Text(Traslacion.c_str());

			std::ostringstream sss;
			std::ostringstream ssd;
			std::ostringstream ssc;
			x = ActiveCamara->GetFront().x;
			y = ActiveCamara->GetRigth().y;
			z = ActiveCamara->GetFront().z;
			sss << x;
			std::string RX(ss.str());
			ssd << y;
			std::string RY(sd.str());
			ssc << z;
			std::string RZ(sc.str());
			std::string Rotacion = "X " + RX + "  Y " + RY + "  Z " + RZ;
			//ImGui::Text("Rotacion:");
			//ImGui::Text(Rotacion.c_str());
			//ImGui::Text("Hola Mundo \n salto de linea");
			ImGui::GetIO().FontGlobalScale;

			ImGui::End();
#endif
			Render();
        }
    }
    CleanupDevice();
    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
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
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    unsigned int width = rc.right - rc.left;
	unsigned int height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	DeviceStruct DSiXL;
#ifdef D3D11
	/*DSiXL.Dev_Flag = createDeviceFlags;
	DSiXL.DriverTypeArr[0] = D3D_DRIVER_TYPE_HARDWARE;
	DSiXL.DriverTypeArr[1] = D3D_DRIVER_TYPE_WARP;
	DSiXL.DriverTypeArr[2] = D3D_DRIVER_TYPE_REFERENCE;
	
DSiXL.FeatureLevel[0] = D3D_FEATURE_LEVEL_11_0;
	DSiXL.FeatureLevel[1] = D3D_FEATURE_LEVEL_10_1;
	DSiXL.FeatureLevel[2] = D3D_FEATURE_LEVEL_10_0;
	DSiXL.numFeatureLevel = ARRAYSIZE(DSiXL.FeatureLevel);
	g_Device->Init(DSiXL);*/
	
	UINT numDriverTypes = ARRAYSIZE(DSiXL.DriverTypeArr);

    SwapChainStruct sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.bufferCount = 1;
    sd.W = width;
    sd.H = height;
    sd.format = FORMAT_R8G8B8A8_UNORM;
    sd.refreshNumerator = 60;
    sd.refreshDenominator = 1;
    sd.bufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.outputWND = (unsigned int)g_hWnd;
    sd.count = 1;
    sd.quality = 0;
    sd.windowed = TRUE;
	g_SwapChain->Init(sd);

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {

        g_Device->DSi.DriverType = g_Device->DSi.DriverTypeArr[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, (D3D_DRIVER_TYPE)g_Device->DSi.DriverType, NULL, g_Device->DSi.Dev_Flag, 
			g_Device->DSi.FeatureLevel, g_Device->DSi.numFeatureLevel,
            D3D11_SDK_VERSION, &g_SwapChain->SwapChainDesc, &g_SwapChain->DXSC, &g_Device->DeviceD11, 
			(D3D_FEATURE_LEVEL*)&g_featureLevel, &g_DeviceContext->m_DeviceContext);

        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;
#endif

    // Create a render target view
#ifdef D3D11
    hr = g_SwapChain->DXSC->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&g_BackBuffer.Texture2D );
#endif
    if( FAILED( hr ) )
        return hr;
#ifdef D3D11
    hr = g_Device->DeviceD11->CreateRenderTargetView(g_BackBuffer.Texture2D, NULL, &g_RenderTargetView.RenderTargetView );
	g_BackBuffer.Texture2D->Release();
#endif
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
#ifdef D3D11
    RenderTargetStruct descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
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


	g_DepthStencil.Init(descDepth);

    hr = g_Device->DeviceD11->CreateTexture2D( &g_DepthStencil.Texture2DDesc, NULL, &g_DepthStencil.Texture2D );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    DepthStencilViewStruct structDepthStencilView;
    ZeroMemory( &structDepthStencilView, sizeof(structDepthStencilView) );
	structDepthStencilView.format = g_DepthStencil.RTS.format;
	structDepthStencilView.viewDimension = DSV_DIMENSION_TEXTURE2D;
	structDepthStencilView.texture2D.mipSlice = 0;
	g_DepthStencilView.Init(structDepthStencilView, (FORMAT)g_DepthStencil.Texture2DDesc.Format);

    hr = g_Device->DeviceD11->CreateDepthStencilView(g_DepthStencil.Texture2D, &g_DepthStencilView.descDSV, &g_DepthStencilView.DepthStencilView );
    if( FAILED( hr ) )
        return hr;
#endif
    // Setup the viewport
    ViewportStruct vp;//Here ViewPort 
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;


	CViewport tempVP;
	tempVP.Init(vp);
#ifdef D3D11
    g_DeviceContext->m_DeviceContext->RSSetViewports( 1, &tempVP.Viewport );

    // Compile the vertex shader
    hr = CompileShaderFromFile( L"Tutorial07.fx", "VS", "vs_4_0", &g_VertexShader.m_pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL, 
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Create the vertex shader
	hr = g_Device->DeviceD11->CreateVertexShader(g_VertexShader.m_pVSBlob->GetBufferPointer(), g_VertexShader.m_pVSBlob->GetBufferSize(), NULL, &g_VertexShader.VertexShader);
    if( FAILED( hr ) )
    {    
		g_VertexShader.m_pVSBlob->Release();
        return hr;
    }
	//Create input layout from compiled VS
	hr = CreateInputLayoutDescFromVertexShaderSignature(g_VertexShader.m_pVSBlob, g_Device->DeviceD11, &g_VertexShader.m_pInputLayout);
	if (FAILED(hr))
		return hr;
    // Set the input layout

    g_DeviceContext->m_DeviceContext->IASetInputLayout(g_VertexShader.m_pInputLayout);
    // Compile the pixel shader
    hr = CompileShaderFromFile( L"Tutorial07.fx", "PS", "ps_4_0", &g_PixelShader.pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }
#endif
    // Create the pixel shader
#ifdef D3D11
    hr = g_Device->DeviceD11->CreatePixelShader(g_PixelShader.pPSBlob->GetBufferPointer(), g_PixelShader.pPSBlob->GetBufferSize(), NULL, &g_PixelShader.PixelShader );
	g_PixelShader.pPSBlob->Release();
#endif
    if( FAILED( hr ) )
        return hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },
    };
	//init data buffer 1
    BufferStruct bd;//buffer parte del vertex
#ifdef D3D11
    ZeroMemory( &bd, sizeof(bd) );
    bd.usage = USAGE_DEFAULT;
	bd.byteWidth = sizeof( SimpleVertex ) * 24;
	bd.bindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.cpuAccessFlags = 0;
	SubresourceData subrsrcData;
	subrsrcData.psysMem = vertices;
	g_VertexBuffer.Init(subrsrcData, bd);

    hr = g_Device->DeviceD11->CreateBuffer( &g_VertexBuffer.BufferDesc, &g_VertexBuffer.SUBDATA, &g_VertexBuffer.VertexBufferD11 );
#endif
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
#ifdef D3D11
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
    g_DeviceContext->m_DeviceContext->IASetVertexBuffers( 0, 1, &g_VertexBuffer.VertexBufferD11, &stride, &offset);
#endif
    // Create index buffer
    // Create vertex buffer
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
	//index buffer
#ifdef D3D11
    bd.usage = USAGE_DEFAULT;
    bd.byteWidth = sizeof( WORD ) * 36;
    bd.bindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.cpuAccessFlags = 0;
	SubresourceData IDS;
	IDS.psysMem = indices;
	g_IndexBuffer.Init(IDS,bd);

    hr = g_Device->DeviceD11->CreateBuffer( &g_IndexBuffer.BufferDesc, &g_IndexBuffer.SUBDATA, &g_IndexBuffer.IndexBufferD11 );
#endif
    if( FAILED( hr ) )
        return hr;

    // Set index buffer
#ifdef D3D11
    g_DeviceContext->m_DeviceContext->IASetIndexBuffer( g_IndexBuffer.IndexBufferD11, DXGI_FORMAT_R16_UINT, 0 );
#endif
    // Set primitive topology
#ifdef D3D11
    g_DeviceContext->m_DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
#endif
    // Create the constant buffers
	//NeverChange
#ifdef D3D11
    bd.usage = USAGE_DEFAULT;
    bd.byteWidth = sizeof(CBNeverChanges);
    bd.bindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.cpuAccessFlags = 0;
	g_NeverChange.Init(bd);
    hr = g_Device->DeviceD11->CreateBuffer( &g_NeverChange.BufferDesc, NULL, &g_NeverChange.BufferD11 );

	//Free Camera
	g_MyCamara.m_CBNeverChanges.Init(bd);
	hr = g_Device->DeviceD11->CreateBuffer(&g_MyCamara.m_CBNeverChanges.BufferDesc, NULL, &g_MyCamara.m_CBNeverChanges.BufferD11);
	if (FAILED(hr))
		return hr;

	//FPS Camera
	g_MyCamaraFP.m_CBNeverChanges.Init(bd);
	hr = g_Device->DeviceD11->CreateBuffer(&g_MyCamaraFP.m_CBNeverChanges.BufferDesc, NULL, &g_MyCamaraFP.m_CBNeverChanges.BufferD11);
	if (FAILED(hr))
		return hr;

	//ChangeOnResize
	bd.byteWidth = sizeof(CBChangeOnResize);
	//Free Camera
	g_MyCamara.m_CBChangesOnResize.Init(bd);
	hr = g_Device->DeviceD11->CreateBuffer(&g_MyCamara.m_CBChangesOnResize.BufferDesc, NULL, &g_MyCamara.m_CBChangesOnResize.BufferD11);
	if (FAILED(hr))
		return hr;

	//FPS Camera
	g_MyCamaraFP.m_CBChangesOnResize.Init(bd);
	hr = g_Device->DeviceD11->CreateBuffer(&g_MyCamaraFP.m_CBChangesOnResize.BufferDesc, NULL, &g_MyCamaraFP.m_CBChangesOnResize.BufferD11);
	if (FAILED(hr))
		return hr;

	//ChangeEveryFrame
	bd.byteWidth = sizeof(CBChangesEveryFrame);
	//Free Camera
	g_MyCamara.m_CBChangesEveryFrame.Init(bd);
	hr = g_Device->DeviceD11->CreateBuffer(&g_MyCamara.m_CBChangesEveryFrame.BufferDesc, NULL, &g_MyCamara.m_CBChangesEveryFrame.BufferD11);
	if (FAILED(hr))
		return hr;

	//FPS Camera
	g_MyCamaraFP.m_CBChangesEveryFrame.Init(bd);
	hr = g_Device->DeviceD11->CreateBuffer(&g_MyCamaraFP.m_CBChangesEveryFrame.BufferDesc, NULL, &g_MyCamaraFP.m_CBChangesEveryFrame.BufferD11);
	if (FAILED(hr))
		return hr;

	// Load the Texture 
	hr = D3DX11CreateShaderResourceViewFromFile(g_Device->DeviceD11, L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
	if (FAILED(hr))
		return hr;
#endif
	// Create the sample state
	SamplerStateStruct samplerDsc;
	ZeroMemory(&samplerDsc, sizeof(samplerDsc));
	samplerDsc.filter = FILTER_MIN_MAG_MIP_LINEAR;
	samplerDsc.addresU = TEXTURE_ADDRESS_WRAP;
	samplerDsc.addresV = TEXTURE_ADDRESS_WRAP;
	samplerDsc.addresW = TEXTURE_ADDRESS_WRAP;
	samplerDsc.comparisonFunc = COMPARISON_NEVER;
	samplerDsc.minLOD = 0;
	samplerDsc.maxLOD = D3D11_FLOAT32_MAX;

	g_SamplerState.Init(samplerDsc);
#ifdef D3D11
	hr = g_Device->DeviceD11->CreateSamplerState(&g_SamplerState.Desc, &g_SamplerState.SamplerStates);
	if (FAILED(hr))
		return hr;
#endif
	// Initialize the world matrix
	g_World = glm::mat4(1.0f);

	// Initialize VM and PM of cameras
	MyStructCamera MyDesc;
	MyDesc.Position = { 0.f, 5.f, -6.f };
	MyDesc.LookAt = { 0.f, 1.f, 0.f };
	MyDesc.Up = { 0.f, 1.f, 0.f };
	MyDesc.FOV = PIDIV4;
	MyDesc.Width = width;
	MyDesc.height = height;
	MyDesc.Near = 0.01f;
	MyDesc.Far = 100.f;

	g_MyCamara.Init(MyDesc);
	g_MyCamaraFP.Init(MyDesc);

	// Set Free Camera VM

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = g_MyCamara.ViewMatrix;
#ifdef D3D11
	g_DeviceContext->m_DeviceContext->UpdateSubresource(g_MyCamara.m_CBNeverChanges.BufferD11, 0, NULL, &cbNeverChanges, 0, 0);
#endif // D3D11	

	//Set FPS Camera VM
	CBNeverChanges cbNeverChanges2;
	cbNeverChanges2.mView = g_MyCamaraFP.ViewMatrix;
#ifdef D3D11
	g_DeviceContext->m_DeviceContext->UpdateSubresource(g_MyCamaraFP.m_CBNeverChanges.BufferD11, 0, NULL, &cbNeverChanges2, 0, 0);
#endif // D3D11

	// Initialize Free Camera PM    
	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = g_MyCamara.Proyeccion;
#ifdef D3D11
	g_DeviceContext->m_DeviceContext->UpdateSubresource(g_MyCamara.m_CBChangesOnResize.BufferD11, 0, NULL, &cbChangesOnResize, 0, 0);
#endif

	//Initialize FPS Camera PM
	CBChangeOnResize cbChangesOnResize2;
	cbChangesOnResize2.mProjection = g_MyCamaraFP.Proyeccion;
#ifdef D3D11
	g_DeviceContext->m_DeviceContext->UpdateSubresource(g_MyCamaraFP.m_CBChangesOnResize.BufferD11, 0, NULL, &cbChangesOnResize2, 0, 0);
#endif

	//Initialize texture, SRV and RTV for inactive camera

	//Texture
#ifdef D3D11
	RenderTargetStruct D;
	ZeroMemory(&D, sizeof(D));
	D.W = width;
	D.H = height;
	D.mipLevels = D.arraySize = 1;
	D.format = FORMAT_R8G8B8A8_UNORM;
	D.sampleDesc.count = 1;
	D.usage = USAGE_DEFAULT;
	D.flags = 8 | 32;		//D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	D.cpuAccessFlags = 65536;//D3D11_CPU_ACCESS_WRITE;
	D.miscFlags = 0;

	g_InteractiveCamaraTexure.Init(D);

	hr = g_Device->DeviceD11->CreateTexture2D(&g_InteractiveCamaraTexure.Texture2DDesc, NULL, &g_InteractiveCamaraTexure.Texture2D);
	if (FAILED(hr))
		return hr;


	//Render Target View
	RenderTargetViewStruct rtDesc;
	rtDesc.format = D.format;
	rtDesc.viewDimension = RTV_DIMENSION_TEXTURE2D;
	rtDesc.texture2D.mipSlice = 0;

	g_RenderTargedView2.Init(rtDesc);

	hr = g_Device->DeviceD11->CreateRenderTargetView(g_InteractiveCamaraTexure.Texture2D, &g_RenderTargedView2.m_Desc, &g_RenderTargedView2.RenderTargetView);
	if (FAILED(hr))
		return hr;
#endif

#ifdef D3D11
	D3D11_SHADER_RESOURCE_VIEW_DESC view;
	view.Format = (DXGI_FORMAT)D.format;
	view.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view.Texture2D.MostDetailedMip = 0;
	view.Texture2D.MipLevels = 1;

	hr = g_Device->DeviceD11->CreateShaderResourceView(g_InteractiveCamaraTexure.Texture2D, &view, &g_TextureInactive);
	if (FAILED(hr))
		return hr;
#endif
	//Set active and inactive camera
	ActiveCamara = &g_MyCamara;
	InactiveCamara = &g_MyCamaraFP;

#ifdef D3D11
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(g_Device->DeviceD11, g_DeviceContext->m_DeviceContext);
	ImGui::StyleColorsDark();
#endif
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
#ifdef D3D11
    if( g_DeviceContext->m_DeviceContext ) g_DeviceContext->m_DeviceContext->ClearState();

    if(g_SamplerState.SamplerStates) g_SamplerState.SamplerStates->Release();
    if( g_pTextureRV ) g_pTextureRV->Release();
    if( g_NeverChange.BufferD11 ) g_NeverChange.BufferD11->Release();
    if(g_ChangeOnResize.BufferD11) g_ChangeOnResize.BufferD11->Release();
    if( g_ChangeEveryFrame.BufferD11 ) g_ChangeEveryFrame.BufferD11->Release();
    if( g_VertexBuffer.VertexBufferD11 ) g_VertexBuffer.VertexBufferD11->Release();
    if( g_IndexBuffer.IndexBufferD11 ) g_IndexBuffer.IndexBufferD11->Release();
    if(g_InputLayout.InputLayout) g_InputLayout.InputLayout->Release();
    if(g_VertexShader.VertexShader) g_VertexShader.VertexShader->Release();
    if(g_PixelShader.PixelShader) g_PixelShader.PixelShader->Release();
    if( g_DepthStencil.Texture2D ) g_DepthStencil.Texture2D->Release();
    if( g_DepthStencilView.DepthStencilView ) g_DepthStencilView.DepthStencilView->Release();
    if( g_RenderTargetView.RenderTargetView ) g_RenderTargetView.RenderTargetView->Release();
    if(g_SwapChain->DXSC) g_SwapChain->DXSC->Release();
    if( g_DeviceContext->m_DeviceContext ) g_DeviceContext->m_DeviceContext->Release();
    if(g_Device->DeviceD11) g_Device->DeviceD11->Release();
#endif
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) {
		return true;
	}

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
#ifdef D3D11
		if (g_DeviceContext->m_DeviceContext != nullptr)
		{
			RECT rc;
			GetClientRect(g_hWnd, &rc);
			UINT width = rc.right - rc.left;
			UINT height = rc.bottom - rc.top;

			g_World = glm::mat4(1.0f);

			ActiveCamara->Set_Height(height);
			ActiveCamara->Set_Width(width);

			CBChangeOnResize cbChangesOnResize;
			cbChangesOnResize.mProjection = ActiveCamara->Proyeccion;

			g_DeviceContext->m_DeviceContext->UpdateSubresource(ActiveCamara->m_CBChangesOnResize.BufferD11, 0, NULL, &cbChangesOnResize, 0, 0);

			InactiveCamara->Set_Height(height);
			InactiveCamara->Set_Width(width);
			InactiveCamara->UpdateProyection();
			CBChangeOnResize cbChangesOnResize2;
			cbChangesOnResize2.mProjection = InactiveCamara->Proyeccion;

			g_DeviceContext->m_DeviceContext->UpdateSubresource(InactiveCamara->m_CBChangesOnResize.BufferD11, 0, NULL, &cbChangesOnResize2, 0, 0);

			if (g_SwapChain->DXSC)
			{
				HRESULT h;
				//Release inactive camera texture, SRV and RTV
				g_InteractiveCamaraTexure.Texture2D->Release();
				g_TextureInactive->Release();
				g_RenderTargedView2.RenderTargetView->Release();

				//Resize inactive camera texture					
				RenderTargetStruct T;
				ZeroMemory(&T, sizeof(T));
				T.W = width;
				T.H = height;
				T.mipLevels = T.arraySize = 1;
				T.format = FORMAT_R8G8B8A8_UNORM;
				T.sampleDesc.count = 1;
				T.usage = USAGE_DEFAULT;
				T.flags = 8 | 32;			// D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
				T.cpuAccessFlags = 65536;	//D3D11_CPU_ACCESS_WRITE;
				T.miscFlags = 0;
				g_InteractiveCamaraTexure.Init(T);

				h = g_Device->DeviceD11->CreateTexture2D(&g_InteractiveCamaraTexure.Texture2DDesc, NULL, &g_InteractiveCamaraTexure.Texture2D);

				RenderTargetViewStruct RTV;
				RTV.format = T.format;
				RTV.viewDimension = RTV_DIMENSION_TEXTURE2D;
				RTV.texture1D.mipSlice = 0;
				g_RenderTargedView2.Init(RTV);
				h = g_Device->DeviceD11->CreateRenderTargetView(g_InteractiveCamaraTexure.Texture2D, &g_RenderTargedView2.m_Desc, &g_RenderTargedView2.RenderTargetView);

				D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
				SRV.Format = (DXGI_FORMAT)T.format;
				SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				SRV.Texture2D.MostDetailedMip = 0;
				SRV.Texture2D.MipLevels = 1;

				h = g_Device->DeviceD11->CreateShaderResourceView(g_InteractiveCamaraTexure.Texture2D, &SRV, &g_TextureInactive);

				g_DeviceContext->m_DeviceContext->OMSetRenderTargets(0, 0, 0);
				g_RenderTargetView.RenderTargetView->Release();


				h = g_SwapChain->DXSC->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

				CBuffer tempBack;
				h = g_SwapChain->DXSC->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tempBack.BufferD11);
				h = g_Device->DeviceD11->CreateRenderTargetView(tempBack.BufferD11, NULL, &g_RenderTargetView.RenderTargetView);
				tempBack.BufferD11->Release();

				g_DepthStencil.Texture2D->Release();
				RenderTargetStruct DepthDesc;
				DepthDesc.W = width;
				DepthDesc.H = height;
				DepthDesc.mipLevels = 1;
				DepthDesc.arraySize = 1;
				DepthDesc.format = FORMAT_D24_UNORM_S8_UINT;
				DepthDesc.sampleDesc.count = 1;
				DepthDesc.sampleDesc.quality = 0;
				DepthDesc.usage = USAGE_DEFAULT;
				DepthDesc.flags = D3D11_BIND_DEPTH_STENCIL;
				DepthDesc.cpuAccessFlags = 0;
				DepthDesc.miscFlags = 0;

				g_DepthStencil.Init(DepthDesc);

				h = g_Device->DeviceD11->CreateTexture2D(&g_DepthStencil.Texture2DDesc, NULL, &g_DepthStencil.Texture2D);

				DepthStencilViewStruct DSV;
				DSV.format = g_DepthStencil.RTS.format;
				DSV.viewDimension = DSV_DIMENSION_TEXTURE2D;
				DSV.texture2D.mipSlice = 0;

				g_DepthStencilView.DepthStencilView->Release();

				g_DepthStencilView.Init(DSV, (FORMAT)g_DepthStencil.Texture2DDesc.Format);

				h = g_Device->DeviceD11->CreateDepthStencilView(g_DepthStencil.Texture2D, &g_DepthStencilView.descDSV, &g_DepthStencilView.DepthStencilView);

				g_DeviceContext->m_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView.RenderTargetView, g_DepthStencilView.DepthStencilView);

				ViewportStruct V;
				V.Width = width;
				V.Height = height;
				V.MinDepth = 0.f;
				V.MaxDepth = 1.f;
				V.TopLeftX = 0;
				V.TopLeftY = 0;

				CViewport ViewP;
				ViewP.Init(V);
				g_DeviceContext->m_DeviceContext->RSSetViewports(1, &ViewP.Viewport);


			}
			ImGui::GetStyle().ScaleAllSizes(1);
		}
#endif
	}
	break;
	case WM_KEYDOWN:
		ActiveCamara->getKeyPress(wParam);
		break;
	case WM_KEYUP:
		ActiveCamara->getKeyRelease(wParam);
		break;
	case WM_RBUTTONDOWN:
	{
		POINT MousePoint;
		GetCursorPos(&MousePoint);
		ActiveCamara->PosInicial = { MousePoint.x, MousePoint.y, 0.f };
		ActiveCamara->mIsClicked = true;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (ActiveCamara->mIsClicked)
		{
			POINT EndPoint;
			GetCursorPos(&EndPoint);
			ActiveCamara->PosFinal = { EndPoint.x, EndPoint.y, 0.f };
			SetCursorPos(ActiveCamara->PosInicial.x, ActiveCamara->PosInicial.y);
			ActiveCamara->Direccion = ActiveCamara->PosInicial - ActiveCamara->PosFinal;
			ActiveCamara->rotate(ActiveCamara->Direccion);
			CBNeverChanges cbNeverChanges;
			cbNeverChanges.mView = ActiveCamara->ViewMatrix;
#ifdef D3D11
			g_DeviceContext->m_DeviceContext->UpdateSubresource(ActiveCamara->m_CBNeverChanges.BufferD11, 0, NULL, &cbNeverChanges, 0, 0);
#endif
		}
		break;
	}
	case WM_RBUTTONUP:
		ActiveCamara->mIsClicked = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
#ifdef D3D11
	// Update our time
	static float t = 0.0f;
	if (g_Device->DSi.DriverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}

	if (ActiveCamara->mForward || ActiveCamara->mBack || ActiveCamara->mLeft || ActiveCamara->mRight || ActiveCamara->mUp || ActiveCamara->mDown || ActiveCamara->mRotateLeft || ActiveCamara->mRotateRight)//DEBO QUITARLO
	{
		ActiveCamara->move();
		ActiveCamara->rotate();
		CBNeverChanges cbNeverChanges;
		cbNeverChanges.mView = ActiveCamara->ViewMatrix;
		g_DeviceContext->m_DeviceContext->UpdateSubresource(ActiveCamara->m_CBNeverChanges.BufferD11, 0, NULL, &cbNeverChanges, 0, 0);
	}

	// Rotate cube around the origin
	//g_World = XMMatrixRotationY( t );
	//g_World = glm::rotate(g_World, .001f, {0, 1, 0});

	// Modify the color
	g_MeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
	g_MeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
	g_MeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;


	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

	//Set inactive camera RTV and DSV
	g_DeviceContext->m_DeviceContext->OMSetRenderTargets(1, &g_RenderTargedView2.RenderTargetView, g_DepthStencilView.DepthStencilView);
	g_DeviceContext->m_DeviceContext->ClearRenderTargetView(g_RenderTargedView2.RenderTargetView, ClearColor);
	g_DeviceContext->m_DeviceContext->ClearDepthStencilView(g_DepthStencilView.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	for (int i = 0; i < 32; i++) {

		g_World = glm::mat4(1.f);
		g_World = glm::translate(g_World, Laberinto_Array[i]);

		CBChangesEveryFrame cb;
		cb.mWorld = glm::transpose(g_World);
		cb.vMeshColor = g_MeshColor;
		g_DeviceContext->m_DeviceContext->UpdateSubresource(InactiveCamara->m_CBChangesEveryFrame.BufferD11, 0, NULL, &cb, 0, 0);

		g_DeviceContext->m_DeviceContext->VSSetShader(g_VertexShader.VertexShader, NULL, 0);
		g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(0, 1, &InactiveCamara->m_CBNeverChanges.BufferD11);
		g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(1, 1, &InactiveCamara->m_CBChangesOnResize.BufferD11);
		g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(2, 1, &InactiveCamara->m_CBChangesEveryFrame.BufferD11);
		g_DeviceContext->m_DeviceContext->PSSetShader(g_PixelShader.PixelShader, NULL, 0);
		g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(2, 1, &InactiveCamara->m_CBChangesEveryFrame.BufferD11);
		g_DeviceContext->m_DeviceContext->PSSetShaderResources(0, 1, &g_pTextureRV);
		g_DeviceContext->m_DeviceContext->PSSetSamplers(0, 1, &g_SamplerState.SamplerStates);
		g_DeviceContext->m_DeviceContext->DrawIndexed(36, 0, 0);
		ID3D11ShaderResourceView* temp = NULL;
		g_DeviceContext->m_DeviceContext->PSSetShaderResources(0, 1, &temp);
	}

	//Set backbuffer and main DSV
	g_DeviceContext->m_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView.RenderTargetView, g_DepthStencilView.DepthStencilView);
	g_DeviceContext->m_DeviceContext->ClearRenderTargetView(g_RenderTargetView.RenderTargetView, ClearColor);
	g_DeviceContext->m_DeviceContext->ClearDepthStencilView(g_DepthStencilView.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//
	// Update variables that change once per frame
	//
	for (int i = 0; i < 32; i++) {

		g_World = glm::mat4(1.f);
		g_World = glm::translate(g_World, Laberinto_Array[i]);

		CBChangesEveryFrame cb;
		cb.mWorld = glm::transpose(g_World);
		cb.vMeshColor = g_MeshColor;
		g_DeviceContext->m_DeviceContext->UpdateSubresource(ActiveCamara->m_CBChangesEveryFrame.BufferD11, 0, NULL, &cb, 0, 0);

		g_DeviceContext->m_DeviceContext->VSSetShader(g_VertexShader.VertexShader, NULL, 0);
		g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(0, 1, &ActiveCamara->m_CBNeverChanges.BufferD11);
		g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(1, 1, &ActiveCamara->m_CBChangesOnResize.BufferD11);
		g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(2, 1, &ActiveCamara->m_CBChangesEveryFrame.BufferD11);
		g_DeviceContext->m_DeviceContext->PSSetShader(g_PixelShader.PixelShader, NULL, 0);
		g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(2, 1, &ActiveCamara->m_CBChangesEveryFrame.BufferD11);
		g_DeviceContext->m_DeviceContext->PSSetShaderResources(0, 1, &g_TextureInactive);
		g_DeviceContext->m_DeviceContext->PSSetSamplers(0, 1, &g_SamplerState.SamplerStates);
		g_DeviceContext->m_DeviceContext->DrawIndexed(36, 0, 0);
		ID3D11ShaderResourceView* temp = NULL;
		g_DeviceContext->m_DeviceContext->PSSetShaderResources(0, 1, &temp);
	}

	//
	// Present our back buffer to our front buffer
	//
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_SwapChain->DXSC->Present(0, 0);
#endif
}
