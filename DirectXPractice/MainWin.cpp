#include <Windows.h>

	#define MAX_NAME_STRING 256
	#define HInstance() GetModuleHandle(NULL)

	WCHAR	pClassName[MAX_NAME_STRING];
	WCHAR	pWindowName[MAX_NAME_STRING];
	INT		WindowX;
	INT		WindowY;
	INT		WindowWidth;
	INT		WindowHeight;

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {
	
	wcscpy_s(pClassName, TEXT("lvRain"));
	wcscpy_s(pWindowName, TEXT("lvRainMEGA"));
	WindowX = 200;
	WindowY = 200;
	WindowWidth = 1366;
	WindowHeight = 768;

	
	// Register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	// Create Window Instance
	HWND hWnd = CreateWindowEx(0, pClassName, pWindowName, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		WindowX, WindowY, WindowWidth, WindowHeight, nullptr, nullptr, HInstance(), nullptr);

	if (!hWnd) {
		MessageBox(0, L"Failed to create winodw!", 0, 0);
		return (0);
	}

	// Show window
	ShowWindow(hWnd, SW_SHOW);

	// message pump

	return (0);
}