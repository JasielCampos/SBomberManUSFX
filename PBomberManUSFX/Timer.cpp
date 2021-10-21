#include "Timer.h"
#include <chrono>

std::chrono::time_point<std::chrono::steady_clock> start;
std::chrono::duration<float> duration;

void Timer::startTimer() {
	start = std::chrono::steady_clock::now();
}

void Timer::stopTimer() {

	auto timeToEnd = std::chrono::steady_clock::now();
	duration = timeToEnd - start;

	float s = duration.count();
	std::cout << "La duracion del nivel fue " << s << " s " << std::endl;
}
