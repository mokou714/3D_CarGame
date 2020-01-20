#include "CarGameApp.h"

using namespace CarGame;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPSTR cmdLine, _In_ int showCmd)
{
	//unused attributes
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);
	//debug for memory allocation

	CarGameApp theApp(hInstance);

	return theApp.Run();
}