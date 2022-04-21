#include "pch.h"

	WCHAR	pClassName[MAX_NAME_STRING];
	WCHAR	pWindowName[MAX_NAME_STRING];
	INT		WindowX;
	INT		WindowY;
	INT		WindowWidth;
	INT		WindowHeight;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_CLOSE:
			PostQuitMessage(77);
			break;
		case WM_DESTROY:
			PostQuitMessage(78);
			break;
		case WM_KEYDOWN:
			if (wParam == 'F')
				SetWindowText(hWnd, TEXT("lvRainALPHA"));
			break;
		case WM_KEYUP:
			if (wParam == 'F')
				SetWindowText(hWnd, TEXT("lvRainOMEGA"));
			break;
		}
		return (DefWindowProc(hWnd, msg, wParam, lParam));
	}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {
	
	wcscpy_s(pClassName, TEXT("lvRain"));
	wcscpy_s(pWindowName, TEXT("lvRainMEGA"));
	WindowX = 200;
	WindowY = 200;
	WindowWidth = 640;
	WindowHeight = 480;

	
	// Register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
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
	MSG msg = {0};
	BOOL gResult;

	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (gResult == -1 ? -1 : msg.wParam);
}