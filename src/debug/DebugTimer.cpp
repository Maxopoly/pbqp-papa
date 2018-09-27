
#include "debug/DebugTimer.hpp"

namespace pbqppapa {

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

}
