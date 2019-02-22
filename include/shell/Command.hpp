#ifndef SHELL_COMMAND_HPP_
#define SHELL_COMMAND_HPP_

#include <string>

#include "solve/StepByStepSolver.hpp"

namespace pbqppapa {

template<typename T>
class CommandHandler;

/**
 * Used for step by step debugging this is a super class to any command doing something
 */
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

	virtual std::string run(std::string input, CommandHandler<T>* cmdHandler) {return "Should not see this";}

	const std::string getIdentifier() {
		return identifier;
	}
};

}
#endif /* SHELL_COMMAND_HPP_ */
