#pragma once
#ifndef WINDOW_H
# define WINDOW_H

# include "LvRain.h"
# include "LvRainException.h"
# include "Keyboard.h"
# include "Mouse.h"
# include <vector>
# include <optional>
//# include "Graphics.h"

class Window {
public:
	class Exception : public LvRainException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept;
		const char* getType() const noexcept;
		std::string translateErrorCode(HRESULT hr) const noexcept;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorString() const noexcept;
	private:
		HRESULT hr;
	};

private:
	class WindowClass {
	public:
		static const char* getName() noexcept;
		static HINSTANCE getInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr const char* wndClassName = "lv Rain Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void setTitle(const std::string& title);

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
public:
	Keyboard	kbd;
	Mouse		mouse;

private:
	//bool	cursorEndabled = true;
	int		width;
	int		height;
	HWND	hWnd;
	//std::unique_ptr<Graphics>	pGfx;
	//std::vector<BYTE>			rawBuffer;
	//std::string					commandLine;
};

// error exception helper macro
#define LVWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define LVWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())
#endif