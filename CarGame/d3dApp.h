#pragma once

#include <d3d11.h> //including Windows.h
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <d3dcompiler.h> //to compile HLSL
#include <vector>
#include <string>
#include <wrl/client.h>
#include <string>
#include <DirectXMath.h>
#include "timer.h"
#include "Utilities.h"
#include "MouseController.h"
#include "KeyboardController.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")


class d3dApp {

public:
	d3dApp(HINSTANCE instance);
	virtual ~d3dApp();

	virtual int Run();	//run game, main update
	virtual bool Init(); //intialize window and d3d
	virtual void OnResize(); //window size changes
	virtual void UpdateScene(float dt) = 0; //update content every frame, implemented by subclass
	virtual void RenderScene() = 0; //draw content every frame, implemented by subclass

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); //get message from window

protected:
	bool InitMainWindow();
	bool InitD3D();
	bool RegisterDevicesForInput();

protected:
	HINSTANCE m_AppInstance;        
	HWND      m_MainWindow;        
	bool      m_AppPaused;       
	bool      m_Minimized;      
	bool      m_Maximized;      
	bool      m_Resizing;

	timer m_timer;

	//D3D11.0 resources
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;       
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;   
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;   
	D3D11_VIEWPORT m_ScreenViewport;     

	//mouse & keyboard
	std::unique_ptr<CarGame::myMouse> m_pMouse;
	std::unique_ptr<CarGame::myKeyboard> m_pKeyboard;

	//initial attributes
	int m_WindowWidth; 
	int m_WindowHeight;

};