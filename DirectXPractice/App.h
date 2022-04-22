#pragma once
# ifndef APP_H
# define APP_H

# include "Window.h"
# include "LvRainTimer.h"
# include <optional>

class App
{
public:
	App();
	int Go();
	~App();
private:
	void DoFrame();
private:
	Window wnd;
	LvRainTimer timer;
};

#endif