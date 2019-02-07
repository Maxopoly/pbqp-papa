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
class StepForwardCommand: public Command<T> {

public:
	StepForwardCommand() :
			Command<T>("forward") {
	}

	~StepForwardCommand() {
	}

	std::string run(std::string input, CommandHandler<T>* cmdHandler) {
		int count;
		if (input == "") {
			count = 1;
		} else {
			try {
			count = std::stoi(input);
			}
			catch (std::invalid_argument&) {
				return "Invalid number, could not parse " + input;
			}
		}
		std::string result = "";
		std::vector<PBQPNode<InfinityWrapper<T>>*> nodes =
				cmdHandler->getSolver()->stepForward(count);
		for (PBQPNode<InfinityWrapper<T>>* node : nodes) {
			if (node == 0) {
				result += "Nothing was done, reduction already complete\n";
			} else {
				result += "Reduced node " + std::to_string(node->getIndex())
						+ "\n";
			}
		}
		return result;
	}
};

}

#endif /* SHELL_STEPFORWARDCOMMAND_HPP_ */
