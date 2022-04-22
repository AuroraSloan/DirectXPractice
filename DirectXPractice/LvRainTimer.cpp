#include "LvRainTimer.h"

LvRainTimer::LvRainTimer() {
	last = std::chrono::steady_clock::now();
}

float LvRainTimer::Mark() {
	const auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = last - old;
	return (frameTime.count());
}

float LvRainTimer::Peek() const {
	return (std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count());
}