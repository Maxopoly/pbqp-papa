#include "debug/DebugTimer.hpp"

namespace pbqppapa {

DebugTimer::DebugTimer() :
		DebugTimer("Timer") {

}

DebugTimer::DebugTimer(std::string name) :
		name(name) {

}

void DebugTimer::startTimer() {
	start = std::chrono::high_resolution_clock::now();
	isRunning = true;
}

void DebugTimer::stopTimer() {
	end = std::chrono::high_resolution_clock::now();
	isRunning = false;
}

/**
 * Returns duration in seconds
 */
double DebugTimer::getDuration() {
	std::chrono::duration<double> elapsed;
	if (isRunning) {
		elapsed = std::chrono::high_resolution_clock::now() - start;
	} else {
		elapsed = end - start;
	}
	return elapsed.count();
}

std::string DebugTimer::getOutput(bool newLine = true) {
	std::string result(name);
	result += ": ";
	result += std::to_string(getDuration());
	result += " sec";
	if (newLine) {
		result += '\n';
	}
	return result;
}

}
