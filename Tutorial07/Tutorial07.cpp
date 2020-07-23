#include <vector>
#include <iostream>
#include <sstream>
#include "resource.h"
#include "CMyCamara.h"
#include "CMyCameraFP.h"
#include "CDepthStencilView.h"
#include "CInputLayout.h"
#include "CSamplerState.h"

#include "CApiManagerDX11.h"

#include "amgui/imgui.h"
#include "amgui/imgui_impl_dx11.h"
#include "amgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
CApiManagerDX11 * CApiManagerDX11::ApiManagerDX11 = nullptr;
HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;
#ifdef D3D11
ID3D11ShaderResourceView*           g_pTextureRV = NULL;
ID3D11ShaderResourceView*           g_TextureInactive = NULL;
#endif
glm::vec4                           g_MeshColor(0.7f, 0.7f, 0.7f, 1.0f);
CMyCamara							g_MyCamara;
CMyCameraFP							g_MyCamaraFP;
CTexture2D							g_InteractiveCamaraTexure;
CDepthStencilView					g_DepthStencilViewFP;
CRenderTargetView					g_RenderTargedView2;
glm::mat4                           g_World;
DRIVER_TYPE							g_driverType = DRIVER_TYPE_NULL; 
glm::mat4                           g_View;
glm::mat4                           g_Projection;
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
			x = ActiveCamara->GetFOV();
			y = ActiveCamara->GetNear();
			z = ActiveCamara->GetFar();
			sss << x;
			std::string RX(ss.str());
			ImGui::Text("FOV:");
			ImGui::Text(RX.c_str());
			ssd << y;
			std::string RY(sd.str());
			ImGui::Text("Near:");
			ImGui::Text(RY.c_str());
			ssc << z;
			std::string RZ(sc.str());
			ImGui::Text("Far:");
			ImGui::Text(RY.c_str());
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
	//Inicializar Device
	g_ApiManager->initDevice();
	//Inicializar SwapChain
	g_ApiManager->initSwapChain(width, height, g_hWnd);
	//Incializar DeviceContext
	g_ApiManager->initDeviceContext();
    // Create a render target view
	g_ApiManager->GetBuffer();
	g_ApiManager->CreateRenderTargetView();
    // Create depth stencil texture
	g_ApiManager->CreateDepthStencilTexture(width, height);
    // Create the depth stencil view sin apimanager
	g_ApiManager->CreateDepthStencilView();
    // Setup the viewport
	g_ApiManager->SetViewPort(width, height);
	// Compile the vertex shader
	// Create the vertex shader
	g_ApiManager->CreateVertexShader();
	//Create input layout from compiled VS
	g_ApiManager->CreateInputLayout();
    // Set the input layout
	g_ApiManager->SetInputLayout();
    // Compile the pixel shader
	// Create the pixel shader
	g_ApiManager->CreatePixelShader();
    // Create vertex buffer
	g_ApiManager->CreateVertexbuffer();
    // Set vertex buffer
	g_ApiManager->SetVertexBuffer();
    // Create index buffer
	g_ApiManager->CreateIndexBuffer();
    /*WORD indices[] =
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
    bd.usage = USAGE_DEFAULT;
    bd.byteWidth = sizeof( WORD ) * 36;
    bd.bindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.cpuAccessFlags = 0;
	SubresourceData IDS;
	IDS.psysMem = indices;
	g_IndexBuffer.Init(IDS,bd);

    hr = g_ApiManager->m_Device.m_DeviceD11->CreateBuffer( &g_IndexBuffer.BufferDesc, &g_IndexBuffer.SUBDATA, &g_IndexBuffer.IndexBufferD11 );
    if( FAILED( hr ) )
        return hr;*/
    // Set index buffer
	g_ApiManager->SetIndexBuffer();
    // Set primitive topology
	g_ApiManager->SetPrimitiveTopology();
  
#ifdef D3D11
	//Camera Never Change
	BufferStruct bd;
    bd.usage = USAGE_DEFAULT;
    bd.byteWidth = sizeof(CBNeverChanges);
    bd.bindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.cpuAccessFlags = 0;
	//Free Camera
	g_MyCamara.m_CBNeverChanges.Init(bd);
	hr = g_ApiManager->m_Device.m_DeviceD11->CreateBuffer(&g_MyCamara.m_CBNeverChanges.BufferDesc, NULL, &g_MyCamara.m_CBNeverChanges.BufferD11);
	if (FAILED(hr))
		return hr;

	//FPS Camera
	g_MyCamaraFP.m_CBNeverChanges.Init(bd);
	hr = g_ApiManager->m_Device.m_DeviceD11->CreateBuffer(&g_MyCamaraFP.m_CBNeverChanges.BufferDesc, NULL, &g_MyCamaraFP.m_CBNeverChanges.BufferD11);
	if (FAILED(hr))
		return hr;

	//ChangeOnResize
	bd.byteWidth = sizeof(CBChangeOnResize);
	//Free Camera
	g_MyCamara.m_CBChangesOnResize.Init(bd);
	hr = g_ApiManager->m_Device.m_DeviceD11->CreateBuffer(&g_MyCamara.m_CBChangesOnResize.BufferDesc, NULL, &g_MyCamara.m_CBChangesOnResize.BufferD11);
	if (FAILED(hr))
		return hr;

	//FPS Camera
	g_MyCamaraFP.m_CBChangesOnResize.Init(bd);
	hr = g_ApiManager->m_Device.m_DeviceD11->CreateBuffer(&g_MyCamaraFP.m_CBChangesOnResize.BufferDesc, NULL, &g_MyCamaraFP.m_CBChangesOnResize.BufferD11);
	if (FAILED(hr))
		return hr;

	//ChangeEveryFrame
	bd.byteWidth = sizeof(CBChangesEveryFrame);
	//Free Camera
	g_MyCamara.m_CBChangesEveryFrame.Init(bd);
	hr = g_ApiManager->m_Device.m_DeviceD11->CreateBuffer(&g_MyCamara.m_CBChangesEveryFrame.BufferDesc, NULL, &g_MyCamara.m_CBChangesEveryFrame.BufferD11);
	if (FAILED(hr))
		return hr;

	//FPS Camera
	g_MyCamaraFP.m_CBChangesEveryFrame.Init(bd);
	hr = g_ApiManager->m_Device.m_DeviceD11->CreateBuffer(&g_MyCamaraFP.m_CBChangesEveryFrame.BufferDesc, NULL, &g_MyCamaraFP.m_CBChangesEveryFrame.BufferD11);
	if (FAILED(hr))
		return hr;

	// Load the Texture 
	hr = D3DX11CreateShaderResourceViewFromFile(g_ApiManager->m_Device.m_DeviceD11, L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
	if (FAILED(hr))
		return hr;
#endif
	// Create the sample state
	SamplerStateStruct samplerDsc;
	g_ApiManager->CreateSamplerState();
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
	g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(g_MyCamara.m_CBNeverChanges.BufferD11, 0, NULL, &cbNeverChanges, 0, 0);
#endif // D3D11	

	//Set FPS Camera VM
	CBNeverChanges cbNeverChanges2;
	cbNeverChanges2.mView = g_MyCamaraFP.ViewMatrix;
#ifdef D3D11
	g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(g_MyCamaraFP.m_CBNeverChanges.BufferD11, 0, NULL, &cbNeverChanges2, 0, 0);
#endif // D3D11

	// Initialize Free Camera PM    
	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = g_MyCamara.Proyeccion;
#ifdef D3D11
	g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(g_MyCamara.m_CBChangesOnResize.BufferD11, 0, NULL, &cbChangesOnResize, 0, 0);
#endif

	//Initialize FPS Camera PM
	CBChangeOnResize cbChangesOnResize2;
	cbChangesOnResize2.mProjection = g_MyCamaraFP.Proyeccion;
#ifdef D3D11
	g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(g_MyCamaraFP.m_CBChangesOnResize.BufferD11, 0, NULL, &cbChangesOnResize2, 0, 0);
#endif

	//Initialize texture, SRV and RTV for inactive camera

	//Texture
#ifdef D3D11
	Texture2DStruct D;
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

	hr = g_ApiManager->m_Device.m_DeviceD11->CreateTexture2D(&g_InteractiveCamaraTexure.Texture2DDesc, NULL, &g_InteractiveCamaraTexure.Texture2D);
	if (FAILED(hr))
		return hr;


	//Render Target View
	RenderTargetViewStruct rtDesc;
	rtDesc.format = D.format;
	rtDesc.viewDimension = RTV_DIMENSION_TEXTURE2D;
	rtDesc.texture2D.mipSlice = 0;

	g_RenderTargedView2.Init(rtDesc);

	hr = g_ApiManager->m_Device.m_DeviceD11->CreateRenderTargetView(g_InteractiveCamaraTexure.Texture2D, &g_RenderTargedView2.m_Desc, &g_RenderTargedView2.RenderTargetView);
	if (FAILED(hr))
		return hr;
#endif

#ifdef D3D11
	D3D11_SHADER_RESOURCE_VIEW_DESC view;
	view.Format = (DXGI_FORMAT)D.format;
	view.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view.Texture2D.MostDetailedMip = 0;
	view.Texture2D.MipLevels = 1;

	hr = g_ApiManager->m_Device.m_DeviceD11->CreateShaderResourceView(g_InteractiveCamaraTexure.Texture2D, &view, &g_TextureInactive);
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
	ImGui_ImplDX11_Init(g_ApiManager->m_Device.m_DeviceD11, g_ApiManager->m_DeviceContext.m_DeviceContext);
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
    if( g_ApiManager->m_DeviceContext.m_DeviceContext ) g_ApiManager->m_DeviceContext.m_DeviceContext->ClearState();
    if(g_ApiManager->m_SamplerLinear.SamplerStates) g_ApiManager->m_SamplerLinear.SamplerStates->Release();
    if( g_pTextureRV ) g_pTextureRV->Release();
    if(g_ApiManager->m_VertexBuffer.VertexBufferD11 ) g_ApiManager->m_VertexBuffer.VertexBufferD11->Release();
    if(g_ApiManager->m_IndexBuffer.IndexBufferD11 ) g_ApiManager->m_IndexBuffer.IndexBufferD11->Release();
    if(g_ApiManager->m_VertexShader.VertexShader) g_ApiManager->m_VertexShader.VertexShader->Release();
    if(g_ApiManager->m_PixelShader.PixelShader) g_ApiManager->m_PixelShader.PixelShader->Release();
    if(g_ApiManager->m_DepthStencil.Texture2D ) g_ApiManager->m_DepthStencil.Texture2D->Release();
    if( g_ApiManager->m_DepthStencilView.DepthStencilView ) g_ApiManager->m_DepthStencilView.DepthStencilView->Release();
    if( g_ApiManager->m_RendertargetView.RenderTargetView ) g_ApiManager->m_RendertargetView.RenderTargetView->Release();
    if(g_ApiManager->m_SwapChain.DXSC) g_ApiManager->m_SwapChain.DXSC->Release();
    if( g_ApiManager->m_DeviceContext.m_DeviceContext ) g_ApiManager->m_DeviceContext.m_DeviceContext->Release();
    if(g_ApiManager->m_Device.m_DeviceD11) g_ApiManager->m_Device.m_DeviceD11->Release();
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
		if (g_ApiManager->m_DeviceContext.m_DeviceContext != nullptr)
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

			g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(ActiveCamara->m_CBChangesOnResize.BufferD11, 0, NULL, &cbChangesOnResize, 0, 0);

			InactiveCamara->Set_Height(height);
			InactiveCamara->Set_Width(width);
			InactiveCamara->UpdateProyection();
			CBChangeOnResize cbChangesOnResize2;
			cbChangesOnResize2.mProjection = InactiveCamara->Proyeccion;

			g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(InactiveCamara->m_CBChangesOnResize.BufferD11, 0, NULL, &cbChangesOnResize2, 0, 0);

			if (g_ApiManager->m_SwapChain.DXSC)
			{
				HRESULT h;
				//Release inactive camera texture, SRV and RTV
				g_InteractiveCamaraTexure.Texture2D->Release();
				g_TextureInactive->Release();
				g_RenderTargedView2.RenderTargetView->Release();

				//Resize inactive camera texture					
				Texture2DStruct T;
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

				h = g_ApiManager->m_Device.m_DeviceD11->CreateTexture2D(&g_InteractiveCamaraTexure.Texture2DDesc, NULL, &g_InteractiveCamaraTexure.Texture2D);

				RenderTargetViewStruct RTV;
				RTV.format = T.format;
				RTV.viewDimension = RTV_DIMENSION_TEXTURE2D;
				RTV.texture1D.mipSlice = 0;
				g_RenderTargedView2.Init(RTV);
				h = g_ApiManager->m_Device.m_DeviceD11->CreateRenderTargetView(g_InteractiveCamaraTexure.Texture2D, &g_RenderTargedView2.m_Desc, &g_RenderTargedView2.RenderTargetView);

				D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
				SRV.Format = (DXGI_FORMAT)T.format;
				SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				SRV.Texture2D.MostDetailedMip = 0;
				SRV.Texture2D.MipLevels = 1;

				h = g_ApiManager->m_Device.m_DeviceD11->CreateShaderResourceView(g_InteractiveCamaraTexure.Texture2D, &SRV, &g_TextureInactive);

				g_ApiManager->m_DeviceContext.m_DeviceContext->OMSetRenderTargets(0, 0, 0);
				g_ApiManager->m_RendertargetView.RenderTargetView->Release();


				h = g_ApiManager->m_SwapChain.DXSC->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

				CBuffer tempBack;
				h = g_ApiManager->m_SwapChain.DXSC->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tempBack.BufferD11);
				h = g_ApiManager->m_Device.m_DeviceD11->CreateRenderTargetView(tempBack.BufferD11, NULL, &g_ApiManager->m_RendertargetView.RenderTargetView);
				tempBack.BufferD11->Release();

				g_ApiManager->m_DepthStencil.Texture2D->Release();
				Texture2DStruct DepthDesc;
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

				g_ApiManager->m_DepthStencil.Init(DepthDesc);

				h = g_ApiManager->m_Device.m_DeviceD11->CreateTexture2D(&g_ApiManager->m_DepthStencil.Texture2DDesc, NULL, &g_ApiManager->m_DepthStencil.Texture2D);

				DepthStencilViewStruct DSV;
				DSV.format = g_ApiManager->m_DepthStencil.RTS.format;
				DSV.viewDimension = DSV_DIMENSION_TEXTURE2D;
				DSV.texture2D.mipSlice = 0;

				g_ApiManager->m_DepthStencilView.DepthStencilView->Release();

				g_ApiManager->m_DepthStencilView.Init(DSV, (FORMAT)g_ApiManager->m_DepthStencil.Texture2DDesc.Format);

				h = g_ApiManager->m_Device.m_DeviceD11->CreateDepthStencilView(g_ApiManager->m_DepthStencil.Texture2D, 
					&g_ApiManager->m_DepthStencilView.descDSV, &g_ApiManager->m_DepthStencilView.DepthStencilView);

				g_ApiManager->m_DeviceContext.m_DeviceContext->OMSetRenderTargets(1, &g_ApiManager->m_RendertargetView.RenderTargetView, 
					g_ApiManager->m_DepthStencilView.DepthStencilView);

				ViewportStruct V;
				V.Width = width;
				V.Height = height;
				V.MinDepth = 0.f;
				V.MaxDepth = 1.f;
				V.TopLeftX = 0;
				V.TopLeftY = 0;

				CViewport ViewP;
				ViewP.Init(V);
				g_ApiManager->m_DeviceContext.m_DeviceContext->RSSetViewports(1, &ViewP.Viewport);


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
			g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(ActiveCamara->m_CBNeverChanges.BufferD11, 0, NULL, &cbNeverChanges, 0, 0);
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
	if (g_ApiManager->m_Device.m_DS.DriverType == D3D_DRIVER_TYPE_REFERENCE)
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
		g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(ActiveCamara->m_CBNeverChanges.BufferD11, 0, NULL, &cbNeverChanges, 0, 0);
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
	g_ApiManager->m_DeviceContext.m_DeviceContext->OMSetRenderTargets(1, &g_RenderTargedView2.RenderTargetView, g_ApiManager->m_DepthStencilView.DepthStencilView);
	g_ApiManager->m_DeviceContext.m_DeviceContext->ClearRenderTargetView(g_RenderTargedView2.RenderTargetView, ClearColor);
	g_ApiManager->m_DeviceContext.m_DeviceContext->ClearDepthStencilView(g_ApiManager->m_DepthStencilView.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	for (int i = 0; i < 32; i++) {

		g_World = glm::mat4(1.f);
		g_World = glm::translate(g_World, Laberinto_Array[i]);

		CBChangesEveryFrame cb;
		cb.mWorld = glm::transpose(g_World);
		cb.vMeshColor = g_MeshColor;
		g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(InactiveCamara->m_CBChangesEveryFrame.BufferD11, 0, NULL, &cb, 0, 0);

		g_ApiManager->m_DeviceContext.m_DeviceContext->VSSetShader(g_ApiManager->m_VertexShader.VertexShader, NULL, 0);
		g_ApiManager->m_DeviceContext.m_DeviceContext->VSSetConstantBuffers(0, 1, &InactiveCamara->m_CBNeverChanges.BufferD11);
		g_ApiManager->m_DeviceContext.m_DeviceContext->VSSetConstantBuffers(1, 1, &InactiveCamara->m_CBChangesOnResize.BufferD11);
		g_ApiManager->m_DeviceContext.m_DeviceContext->VSSetConstantBuffers(2, 1, &InactiveCamara->m_CBChangesEveryFrame.BufferD11);
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetShader(g_ApiManager->m_PixelShader.PixelShader, NULL, 0);
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetConstantBuffers(2, 1, &InactiveCamara->m_CBChangesEveryFrame.BufferD11);
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetShaderResources(0, 1, &g_pTextureRV);
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetSamplers(0, 1, &g_ApiManager->m_SamplerLinear.SamplerStates);
		g_ApiManager->m_DeviceContext.m_DeviceContext->DrawIndexed(36, 0, 0);
		ID3D11ShaderResourceView* temp = NULL;
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetShaderResources(0, 1, &temp);
	}

	//Set backbuffer and main DSV
	g_ApiManager->m_DeviceContext.m_DeviceContext->OMSetRenderTargets(1, &g_ApiManager->m_RendertargetView.RenderTargetView, g_ApiManager->m_DepthStencilView.DepthStencilView);
	g_ApiManager->m_DeviceContext.m_DeviceContext->ClearRenderTargetView(g_ApiManager->m_RendertargetView.RenderTargetView, ClearColor);
	g_ApiManager->m_DeviceContext.m_DeviceContext->ClearDepthStencilView(g_ApiManager->m_DepthStencilView.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//
	// Update variables that change once per frame
	//
	for (int i = 0; i < 32; i++) {

		g_World = glm::mat4(1.f);
		g_World = glm::translate(g_World, Laberinto_Array[i]);

		CBChangesEveryFrame cb;
		cb.mWorld = glm::transpose(g_World);
		cb.vMeshColor = g_MeshColor;
		g_ApiManager->m_DeviceContext.m_DeviceContext->UpdateSubresource(ActiveCamara->m_CBChangesEveryFrame.BufferD11, 0, NULL, &cb, 0, 0);

		g_ApiManager->m_DeviceContext.m_DeviceContext->VSSetShader(g_ApiManager->m_VertexShader.VertexShader, NULL, 0);
		g_ApiManager->m_DeviceContext.m_DeviceContext->VSSetConstantBuffers(0, 1, &ActiveCamara->m_CBNeverChanges.BufferD11);
		g_ApiManager->m_DeviceContext.m_DeviceContext->VSSetConstantBuffers(1, 1, &ActiveCamara->m_CBChangesOnResize.BufferD11);
		g_ApiManager->m_DeviceContext.m_DeviceContext->VSSetConstantBuffers(2, 1, &ActiveCamara->m_CBChangesEveryFrame.BufferD11);
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetShader(g_ApiManager->m_PixelShader.PixelShader, NULL, 0);
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetConstantBuffers(2, 1, &ActiveCamara->m_CBChangesEveryFrame.BufferD11);
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetShaderResources(0, 1, &g_TextureInactive);
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetSamplers(0, 1, &g_ApiManager->m_SamplerLinear.SamplerStates);
		g_ApiManager->m_DeviceContext.m_DeviceContext->DrawIndexed(36, 0, 0);
		ID3D11ShaderResourceView* temp = NULL;
		g_ApiManager->m_DeviceContext.m_DeviceContext->PSSetShaderResources(0, 1, &temp);
	}

	//
	// Present our back buffer to our front buffer
	//
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_ApiManager->m_SwapChain.DXSC->Present(0, 0);
#endif
}
