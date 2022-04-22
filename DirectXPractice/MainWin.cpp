#include "Window.h"
#include "App.h"
#include <cstdlib>

static const wchar_t* wConv(char* str) {
	const size_t csize = strlen(str) + 1;

}
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {
	try
	{
		return (App{}.Go());
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