#ifndef DEBUG_DEBUGTIMER_HPP_
#define DEBUG_DEBUGTIMER_HPP_

#include <string>
#include <chrono>

namespace pbqppapa {

class DebugTimer {

private:
	std::clock_t start;
	std::clock_t end;
	std::string name;

public:

	DebugTimer();

	DebugTimer(std::string name);

	void startTimer();

	void stopTimer();

	double getDuration();

	std::string getOutput();

};

}

#endif /* DEBUG_DEBUGTIMER_HPP_ */
