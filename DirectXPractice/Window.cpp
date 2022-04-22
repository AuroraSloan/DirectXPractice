#include "Window.h"
#


// ==== Exception ==== //

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	: LvRainException(line, file), hr(hr) {}

const char* Window::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Error Code] " << getErrorCode() << std::endl
		<< "[Description] " << getErrorString() << std::endl
		<< getOriginString();
	whatBuffer = oss.str();
	return (whatBuffer.c_str());
}

const char* Window::Exception::getType() const noexcept {
	return ("Lv Window Exception");
}

std::string Window::Exception::translateErrorCode(HRESULT hr) const noexcept {
	char*	pMsgBuf = nullptr;
	DWORD	nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr);
	if (nMsgLen == 0)
		return ("Unidentified error code");
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return (errorString);
}

HRESULT Window::Exception::getErrorCode() const noexcept {
	return (hr);
}
std::string  Window::Exception::getErrorString() const noexcept {
	return (translateErrorCode(hr));
}

// ==== Window Class ====//
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
	wc.lpszClassName = reinterpret_cast<LPCWSTR>(getName());
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() noexcept {
	UnregisterClass(reinterpret_cast<LPCWSTR>(wndClassName), getInstance());
}

const char* Window::WindowClass::getName() noexcept {
	return (wndClassName);
}

HINSTANCE Window::WindowClass::getInstance() noexcept {
	return (wndClass.hInst);
}


// ==== Window ====//

Window::Window(int width, int height, const wchar_t* name) : width(width), height(height) {
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	//adjust wr draw area based on style options (TF for menu)
	if (!AdjustWindowRect(&wr, DWSTYLE, FALSE))
		throw LVWND_LAST_EXCEPT();

	hWnd = CreateWindow(reinterpret_cast<LPCWSTR>(WindowClass::getName()), name, DWSTYLE, CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::getInstance(), this);

	if (!hWnd)
		throw LVWND_LAST_EXCEPT();

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

void Window::setTitle(const std::string& title) {
	wchar_t newTitle[MAX_NAME_STRING + 1];
	size_t tsz = strlen(title.c_str());
	if (mbstowcs_s(&tsz, newTitle, MAX_NAME_STRING, title.c_str(), MAX_NAME_STRING - 1) != 0)
		errno = EINVAL;
	if (SetWindowText(hWnd, newTitle) == 0)
		throw LVWND_LAST_EXCEPT();
}

std::optional<int> Window::processMessages() noexcept {
	MSG msg {0};

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT)
			return ((int)msg.wParam);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
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

	case WM_KILLFOCUS:
		kbd.clearState();
		break;

		/*********** KEYBOARD MESSAGES ***********/
	case WM_KEYDOWN:
		// syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
		//stifle this keyboard message if imgui wants to capture
		/*if (imio.WantCaptureKeyboard)
			break;*/
		if (!(lParam & 0x40000000) || kbd.autorepeatIsEnabled()) // filter autorepeat
			kbd.onKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		// stifle this keyboard message if imgui wants to capture
		/*if (imio.WantCaptureKeyboard)
			break;*/
		kbd.onKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.onChar(static_cast<unsigned char>(wParam));
		// stifle this keyboard message if imgui wants to capture
		/*if (imio.WantCaptureKeyboard)
		{
			break;
		}
		kbd.OnChar(static_cast<unsigned char>(wParam));*/
		break;
		/*********** END KEYBOARD MESSAGES ***********/

		/************* MOUSE MESSAGES ****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// cursorless exclusive gets first dibs
		/*if (!cursorEnabled)
		{
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
				HideCursor();
			}
			break;
		}
		// stifle this mouse message if imgui wants to capture
		if (imio.WantCaptureMouse)
		{
			break;
		}*/
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		/*SetForegroundWindow(hWnd);
		if (!cursorEnabled)
		{
			ConfineCursor();
			HideCursor();
		}
		// stifle this mouse message if imgui wants to capture
		if (imio.WantCaptureMouse)
		{
			break;
		}*/
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		// stifle this mouse message if imgui wants to capture
		/*if (imio.WantCaptureMouse)
		{
			break;
		}*/
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		// stifle this mouse message if imgui wants to capture
		/*if (imio.WantCaptureMouse)
		{
			break;
		}*/
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		// release mouse if outside of window
		/*if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}*/
		break;
	}
	case WM_RBUTTONUP:
	{
		// stifle this mouse message if imgui wants to capture
		/*if (imio.WantCaptureMouse)
		{
			break;
		}*/
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		// release mouse if outside of window
		/*if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}*/
		break;
	}
	case WM_MOUSEWHEEL:
	{
		// stifle this mouse message if imgui wants to capture
		/*if (imio.WantCaptureMouse)
		{
			break;
		}*/
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/************** END MOUSE MESSAGES **************/
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}