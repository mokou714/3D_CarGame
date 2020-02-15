#pragma once
/*
	utility menthods for implementing game and d3d app
*/
#include <d3dcompiler.h>
#define _WIN32_WINNT 0x600
#pragma comment(lib,"d3dcompiler.lib")

HRESULT CompileShader(const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut);
void CheckIfFailed(HRESULT hr);


