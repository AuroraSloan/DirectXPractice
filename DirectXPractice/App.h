#pragma once
# ifndef APP_H
# define APP_H

# include "Window.h"

class App
{
public:
	App();
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
};

#endif