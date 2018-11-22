#ifndef SHELL_COMMAND_HPP_
#define SHELL_COMMAND_HPP_

#include <string>

#include "StepByStepSolver.hpp"

namespace pbqppapa {

template<typename T>
class StepByStepSolver;

template<typename T>
class Command {
private:

	std::string identifier;

public:

	Command(std::string identifier) :
			identifier(identifier) {
	}

	virtual ~Command() {
	}

	virtual std::string run(std::string input, StepByStepSolver<T> solver) {return "";}

	const std::string getIdentifier() {
		return identifier;
	}
};

}
#endif /* SHELL_COMMAND_HPP_ */
