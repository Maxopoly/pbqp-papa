#ifndef SHELL_DUMPCOMMAND_HPP_
#define SHELL_DUMPCOMMAND_HPP_

#include "shell/Command.hpp"

namespace pbqppapa {

template<typename T>
class Command;

template<typename T>
class DumpCommand : public Command<T> {

	DumpCommand() :
		Command<T>("dump") {
	}

	 ~DumpCommand() {
	}

	 std::string run(std::string input, StepByStepSolver<T> solver) {
		 //TODO check/sanitize path
		 solver.dump(input);
		 return "Output to " + input;
	 }

};

}

#endif /* SHELL_DUMPCOMMAND_HPP_ */
