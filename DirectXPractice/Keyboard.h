#pragma once

#ifndef KEYBOARD_H
# define KEYBOARD_H

# include <queue>
# include <bitset>
class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event() noexcept : type(Type::Invalid), code(0u) {}
		Event(Type type, unsigned char code) noexcept : type(type), code(code) {}
		bool IsPress() const noexcept
		{
			return (type == Type::Press);
		}
		bool IsRelease() const noexcept
		{
			return (type == Type::Release);
		}
		bool IsInvalid() const noexcept
		{
			return (type == Type::Invalid);
		}
		unsigned char GetCode() const noexcept
		{
			return (code);
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	// key event stuff
	bool keyIsPressed(unsigned char keycode) const noexcept;
	Event readKey() noexcept;
	bool keyIsEmpty() const noexcept;
	void clearKey() noexcept;
	// char event stuff
	char readChar() noexcept;
	bool charIsEmpty() const noexcept;
	void clearChar() noexcept;
	void clear() noexcept;
	// autorepeat control
	void enableAutorepeat() noexcept;
	void disableAutorepeat() noexcept;
	bool autorepeatIsEnabled() const noexcept;
private:
	void onKeyPressed(unsigned char keycode) noexcept;
	void onKeyReleased(unsigned char keycode) noexcept;
	void onChar(char character) noexcept;
	void clearState() noexcept;
	template<typename T>
	static void trimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};

#endif
