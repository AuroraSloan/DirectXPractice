#include "Window.h"

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {
	Window	wnd(800, 300, TEXT("Windows9000"));
	MSG		msg = {0};
	BOOL	gRes;

	while ((gRes = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (gRes == -1 ? -1 : static_cast<int>(msg.wParam));
}