#ifndef SHELL_STEPFORWARDCOMMAND_HPP_
#define SHELL_STEPFORWARDCOMMAND_HPP_

#include <string>

#include "shell/Command.hpp"

namespace pbqppapa {

template<typename T>
class Command;
template<typename T>
class CommandHandler;

template<typename T>
class StepForwardCommand : public Command<T> {

public:
	StepForwardCommand() :
		Command<T>("forward") {
	}

	 ~StepForwardCommand() {
	}

	 std::string run(std::string input, CommandHandler<T>* cmdHandler) {
		 //TODO multiple at once and select specfic node
		 PBQPNode <InfinityWrapper<T>>* node = cmdHandler->getSolver()->stepForward();
		 if (node == 0) {
			 return "Nothing was done, reduction already complete";
		 }
		 return "Reduced node " + std::to_string(node->getIndex());
	 }
};

}



#endif /* SHELL_STEPFORWARDCOMMAND_HPP_ */
