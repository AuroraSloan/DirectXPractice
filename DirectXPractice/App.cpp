#include "App.h"

App::App() : wnd(800, 600, L"Lv Rain Booster") {}

int	App::Go()
{
	MSG		msg = { 0 };
	BOOL	gRes;

	while ((gRes = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (gRes == -1 ? -1 : static_cast<int>(msg.wParam));
}

void App::DoFrame() {

}