#include "debug/DebugTimer.hpp"

namespace pbqppapa {

DebugTimer::DebugTimer() : DebugTimer("Timer") {

}

DebugTimer::DebugTimer(std::string name) : name(name) {

}

void DebugTimer::startTimer() {
	start = clock();
}

void DebugTimer::stopTimer() {
	end = clock();
}

/**
 * Returns duration in seconds
 */
double DebugTimer::getDuration() {
	return double(end - start) / CLOCKS_PER_SEC;
}

std::string DebugTimer::getOutput() {
	std::string result (name);
	result += ": ";
	result += std::to_string(getDuration());
	result += " sec\n";
	return result;
}

}
