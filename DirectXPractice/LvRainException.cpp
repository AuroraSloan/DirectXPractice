#include "LvRainException.h"

#include <sstream>


LvRainException::LvRainException(int line, const char* file) : line(line), file(file) {}

const char* LvRainException::what() const noexcept
{
	std::stringstream oss;
	oss << getType() << std::endl
		<< getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* LvRainException::getType() const noexcept
{
	return "Lv Rain Exception";
}

int LvRainException::getLine() const noexcept
{
	return line;
}

const std::string& LvRainException::getFile() const noexcept
{
	return file;
}

std::string LvRainException::getOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}