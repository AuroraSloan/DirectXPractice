#pragma once

#ifndef LVRAINEXCEPTION_H
# define LVRAINEXCEPRTION_H

# include <exception>
# include <string>

class LvRainException : public std::exception
{
public:
	LvRainException(int line, const char* file);
	const char* what() const noexcept override;
	virtual const char* getType() const noexcept;
	int getLine() const noexcept;
	const std::string& getFile() const noexcept;
	std::string getOriginString() const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};

#endif