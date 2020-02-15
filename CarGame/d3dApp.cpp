#include "d3dApp.h"
#include <sstream>

/*
*	To review: InitWindow(), InitD3D
*/

//local
namespace
{
	d3dApp* g_pd3dApp = nullptr;
}


LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Handle window message
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before m_hMainWnd is valid.
	return g_pd3dApp->MsgProc(hwnd, msg, wParam, lParam);
}

d3dApp::d3dApp(HINSTANCE hInstance)
	: m_AppInstance(hInstance),
	m_WindowWidth(1200),
	m_WindowHeight(900),
	m_MainWindow(nullptr),
	m_AppPaused(false),
	m_Minimized(false),
	m_Maximized(false),
	m_Resizing(false),
	m_d3dDevice(nullptr),
	m_d3dImmediateContext(nullptr),
	m_SwapChain(nullptr),
	m_DepthStencilBuffer(nullptr),
	m_RenderTargetView(nullptr),
	m_DepthStencilView(nullptr)
{
	ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));
	g_pd3dApp = this;
}

d3dApp::~d3dApp(){
	if (m_d3dImmediateContext)
		m_d3dImmediateContext->ClearState();
}


int d3dApp::Run()
{
	MSG msg = { 0 };

	m_timer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_timer.Tick();

			if (!m_AppPaused)
			{
				UpdateScene(m_timer.DeltaTime());
				RenderScene();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}

bool d3dApp::Init()
{
	//set up mouse, keyboard pointer
	m_pMouse = std::make_unique<CarGame::myMouse>();
	m_pKeyboard = std::make_unique<CarGame::myKeyboard>();


	if (!InitMainWindow())
		return false;

	if (!InitD3D())
		return false;

	if (!RegisterDevicesForInput())
		return false;

	return true;
}

void d3dApp::OnResize()
{
	assert(m_d3dImmediateContext);
	assert(m_d3dDevice);
	assert(m_SwapChain);

	// reset rendering resource
	m_RenderTargetView.Reset();
	m_DepthStencilView.Reset();
	m_DepthStencilBuffer.Reset();

	// reset swap chain and render target view
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	CheckIfFailed(m_SwapChain->ResizeBuffers(1, m_WindowWidth, m_WindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	CheckIfFailed(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	CheckIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RenderTargetView.GetAddressOf()));
	backBuffer.Reset();


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_WindowWidth;
	depthStencilDesc.Height = m_WindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//4xMsaa
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//create depthstencil buffer and view
	CheckIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_DepthStencilBuffer.GetAddressOf()));
	CheckIfFailed(m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf()));

	//bind depthstencil buffer
	m_d3dImmediateContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

	//set viewpost
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast<float>(m_WindowWidth);
	m_ScreenViewport.Height = static_cast<float>(m_WindowHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;
	m_d3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

	//keep cursor on the window
	RECT ClipWindowRect;
	GetWindowRect(m_MainWindow, &ClipWindowRect);
	ClipWindowRect.top += 30;
	ClipCursor(&ClipWindowRect);
}


/*
A Windows window is identified by a "window handle" (HWND) 
and is created after the CWnd object is created by a call to the Create member function of class CWnd.

wParam -> word parameter 16/32/64bits
lParam -> long parameter 32/64bits

*/

LRESULT d3dApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_AppPaused = true;
			m_timer.Stop();
		}
		else
		{
			m_AppPaused = false;
			m_timer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		m_WindowWidth = LOWORD(lParam);
		m_WindowHeight = HIWORD(lParam);
		if (m_d3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_Maximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (m_Minimized)
				{
					m_AppPaused = false;
					m_Minimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (m_Maximized)
				{
					m_AppPaused = false;
					m_Maximized = false;
					OnResize();
				}
				else if (m_Resizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or m_pSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing = true;
		m_timer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing = false;
		m_timer.Start();
		OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

		//****************************************** mouse/keyboard ***********************************************
	case WM_INPUT:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDOWN:

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:

	case WM_MOUSEWHEEL:
	case WM_MOUSEHOVER:
	case WM_MOUSEMOVE:
		m_pMouse->ProcessMessage(msg, wParam, lParam);
		return 0;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		m_pKeyboard->ProcessMessage(msg, wParam, lParam);
		return 0;
		//********************************************************************************************************

	case WM_ACTIVATEAPP:
		m_pMouse->ProcessMessage(msg, wParam, lParam);
		m_pKeyboard->ProcessMessage(msg, wParam, lParam);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
	
}


bool d3dApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_AppInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, "RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, m_WindowWidth, m_WindowHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	m_MainWindow = CreateWindow("D3DWndClassName", "Car Game",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_AppInstance, 0);
	
	if (!m_MainWindow)
	{
		MessageBox(0, "CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(m_MainWindow, SW_SHOW);
	UpdateWindow(m_MainWindow);

	return true;
}

bool d3dApp::InitD3D()
{
	HRESULT hr = S_OK;
	UINT createDeviceFlags = 0;

	// driver types
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// feature levels, do DX11.0 only
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;
	D3D_DRIVER_TYPE d3dDriverType;

	//for each driver type, try to create the d3d device
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		d3dDriverType = driverTypes[driverTypeIndex];

		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[0], numFeatureLevels,
			D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &featureLevel, m_d3dImmediateContext.GetAddressOf());
		
		if (SUCCEEDED(hr))
			break;
	}

	//fail to create device
	if (FAILED(hr)){
		MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	//only use DX11.0 for Win7+ OS
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory = nullptr;	
	
	CheckIfFailed(m_d3dDevice.As(&dxgiDevice));
	CheckIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
	CheckIfFailed(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())));

	//swap chain desc
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = m_WindowWidth;
	sd.BufferDesc.Height = m_WindowHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//disable 4xMsaa
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_MainWindow;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	//create swap chain
	CheckIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &sd, m_SwapChain.GetAddressOf()));
	
	OnResize();

	return true;
}

bool d3dApp::RegisterDevicesForInput() {
	RAWINPUTDEVICE Rid[1];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;

	Rid[0].dwFlags = 0;  
	Rid[0].hwndTarget = m_MainWindow;

	return RegisterRawInputDevices(Rid, 1, sizeof(RAWINPUTDEVICE));

	//Rid[1].usUsagePage = 0x01;
	//Rid[1].usUsage = 0x06;
	//Rid[1].dwFlags = 0;   
	//Rid[1].hwndTarget = m_MainWindow;
}


