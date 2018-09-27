#ifndef DEBUG_DEBUGTIMER_HPP_
#define DEBUG_DEBUGTIMER_HPP_

#include <chrono>

namespace pbqppapa {

class DebugTimer {

private:
	std::clock_t start;
	std::clock_t end;

public:

	void startTimer();

	void stopTimer();

	double getDuration();

};

}

#endif /* DEBUG_DEBUGTIMER_HPP_ */
