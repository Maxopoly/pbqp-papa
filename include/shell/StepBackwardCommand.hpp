#ifndef SHELL_STEPBACKWARDCOMMAND_HPP_
#define SHELL_STEPBACKWARDCOMMAND_HPP_

#include <string>

#include "shell/Command.hpp"

namespace pbqppapa {

template<typename T>
class Command;

template<typename T>
class StepBackwardCommand : public Command<T> {

	StepBackwardCommand() :
		Command<T>("back") {
	}

	 ~StepBackwardCommand() {
	}

	 std::string run(std::string input, StepByStepSolver<T> solver) {
		 //TODO multiple at once
		 PBQPNode <T>* node = solver.stepBackward();
		 if (node == 0) {
			 return "Nothing was done, nothing to revert";
		 }
		 return "Reverted reduction of node " + std::to_string(node->getIndex());
	 }

};

}



#endif /* SHELL_STEPBACKWARDCOMMAND_HPP_ */
