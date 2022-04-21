#include "Window.h"

// Window Class
Window::WindowClass	Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(HInstance()) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() noexcept {
	UnregisterClass(wndClassName, getInstance());
}

const wchar_t* Window::WindowClass::getName() noexcept {
	return (wndClassName);
}

HINSTANCE Window::WindowClass::getInstance() noexcept {
	return (wndClass.hInst);
}


// Window

Window::Window(int width, int height, const wchar_t* name) noexcept {
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	//adjust wr draw area based on style options (TF for menu)
	AdjustWindowRect(&wr, DWSTYLE, FALSE);

	hWnd = CreateWindow(WindowClass::getName(), name, DWSTYLE, CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::getInstance(), this);

	ShowWindow(hWnd, SW_SHOWDEFAULT);

}

Window::~Window() {
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	if (msg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMsgThunk));
		return pWnd->handleMsg(hWnd, msg, wParam, lParam);
	}
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

LRESULT CALLBACK Window::handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return (pWnd->handleMsg(hWnd, msg, wParam, lParam));
}

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return (0);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}