#ifndef DEBUG_DEBUGTIMER_HPP_
#define DEBUG_DEBUGTIMER_HPP_

#include <string>
#include <chrono>

namespace pbqppapa {

/**
 * Utility class for measuring time elapsed. May be reused by starting again after stopping
 */
class DebugTimer {

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
	std::string name;
	bool isRunning = false;

public:

	/**
	 * Creates a new timer. Does not start it
	 */
	DebugTimer();

	/**
	 * Creates a new timer with the given name. Does not start it
	 */
	DebugTimer(std::string name);

	/**
	 * Starts the timer
	 */
	void startTimer();

	/**
	 * Stops the timer
	 */
	void stopTimer();

	/**
	 * Can only be used after starting the timer. Will use the current time if the timer has not been stopped yet.
	 * Returned time is in seconds
	 */
	double getDuration();

	/**
	 * Gets nicely formatted output including the timers name and elapsed time. Will use the current time if the timer has not been stopped yet
	 * Optionally puts \n at the end of the output
	 */
	std::string getOutput(bool newLine);

};

}

#endif /* DEBUG_DEBUGTIMER_HPP_ */
