#include "debug/DebugTimer.hpp"

namespace pbqppapa {

DebugTimer::DebugTimer() : DebugTimer("Timer") {

}

DebugTimer::DebugTimer(std::string name) : name(name) {

}

void DebugTimer::startTimer() {
	start = std::chrono::high_resolution_clock::now();
}

void DebugTimer::stopTimer() {
	end = std::chrono::high_resolution_clock::now();

}

/**
 * Returns duration in seconds
 */
double DebugTimer::getDuration() {
	std::chrono::duration<double> elapsed = end - start;
	return elapsed.count();
}

std::string DebugTimer::getOutput() {
	std::string result (name);
	result += ": ";
	result += std::to_string(getDuration());
	result += " sec\n";
	return result;
}

}
