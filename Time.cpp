#include "Time.hpp"

Time::Time() {

}

Time::~Time() {

}

void Time::start() {

	startTime = std::chrono::high_resolution_clock::now();

}

void Time::stop() {

	stopTime = std::chrono::high_resolution_clock::now();

}

long long int Time::totalTime() {

	long long time = std::chrono::duration_cast<std::chrono::microseconds>(Time::stopTime - Time::startTime).count();
	return time;
}