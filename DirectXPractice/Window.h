#pragma once
#ifndef WINDOW_H
# define WINDOW_H

# include "lvrain.h"
# include <vector>
# include <optional>
//# include "Graphics.h"

class Window {
private:
	class WindowClass {
	public:
		static const wchar_t* getName() noexcept;
		static HINSTANCE getInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr const wchar_t* wndClassName = TEXT("lv Rain Direct3D Engine Window");
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const wchar_t* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	/*void	setTitle(const std::string& title);
	void	enableCursor() noexcept;
	void	diableCursor() noexcept;
	bool	cursorEnabled() const noexcept;
	static std::optional<int> processMessage() noexcept;
	Graphics& Gfx();*/
private:
	/*void	confineCursor() noexcept;
	void	freeCursor() noexcept;
	void	showCursor() noexcept;
	void	hideCursor() noexcept;
	void	enableImGuiMouse() noexcept;
	void	disableImGuiMouse() noexcept;*/
	static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
//public:
	//Keyboard	kbd;
	//Mouse		mouse;

private:
	//bool	cursorEndabled = true;
	int		width;
	int		height;
	HWND	hWnd;
	//std::unique_ptr<Graphics>	pGfx;
	//std::vector<BYTE>			rawBuffer;
	//std::string					commandLine;
};

#endif