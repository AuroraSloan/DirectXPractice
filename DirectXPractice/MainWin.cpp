#include "Window.h"
#include <cstdlib>

static const wchar_t* wConv(char* str) {
	const size_t csize = strlen(str) + 1;

}
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {
	try
	{
		Window	wnd(800, 300, TEXT("Windows9000"));
		MSG		msg = { 0 };
		BOOL	gRes;

		while ((gRes = GetMessage(&msg, nullptr, 0, 0)) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		return (gRes == -1 ? -1 : static_cast<int>(msg.wParam));
	}
	catch (const LvRainException& e)
	{
		wchar_t what[MAX_NAME_STRING+1];
		wchar_t type[MAX_NAME_STRING+1];
		size_t wsz = strlen(e.what());
		size_t tsz = strlen(e.getType());
		if (mbstowcs_s(&wsz, what, MAX_NAME_STRING, e.what(), MAX_NAME_STRING - 1) != 0 ||
			mbstowcs_s(&tsz, type, MAX_NAME_STRING, e.what(), MAX_NAME_STRING - 1) != 0)
			errno = EINVAL;
		MessageBox(nullptr, what, type, MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		wchar_t	what[MAX_NAME_STRING + 1];
		size_t	wsz = strlen(e.what());
		if (mbstowcs_s(&wsz, what, MAX_NAME_STRING, e.what(), MAX_NAME_STRING - 1) != 0)
			errno = EINVAL;
		MessageBox(nullptr, what, L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return (-1);
}