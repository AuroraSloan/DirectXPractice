#pragma once
#ifndef LVRAINTIMER_H
# define LVRAINTIMER_H

# include <chrono>

class LvRainTimer
{
public:
	LvRainTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};

#endif