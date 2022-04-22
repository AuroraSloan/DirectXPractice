#include "App.h"

App::App() : wnd(800, 600, L"Lv Rain Booster") {}

App::~App() {}

int	App::Go(){
	while (true) {
		if (const auto ecode = Window::processMessages())
			return (*ecode);
		DoFrame();
	}
	return {};
}

void App::DoFrame() {
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.setTitle(oss.str());
}