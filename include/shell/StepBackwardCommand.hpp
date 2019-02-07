#ifndef SHELL_STEPBACKWARDCOMMAND_HPP_
#define SHELL_STEPBACKWARDCOMMAND_HPP_

#include <string>

#include "shell/Command.hpp"

namespace pbqppapa {

template<typename T>
class Command;
template<typename T>
class CommandHandler;

template<typename T>
class StepBackwardCommand: public Command<T> {

public:
	StepBackwardCommand() :
			Command<T>("back") {
	}

	~StepBackwardCommand() {
	}

	std::string run(std::string input, CommandHandler<T>* cmdHandler) {
		if (input == "") {
			PBQPNode<InfinityWrapper<T>>* node =
					cmdHandler->getSolver()->stepBackward();
			if (node == 0) {
				return "Nothing was done, nothing to revert";
			}
			return "Reverted reduction of node "
					+ std::to_string(node->getIndex());
		} else {
			int count = std::stoi(input);
			std::string result = "";
			std::vector<PBQPNode<InfinityWrapper<T>>*> nodes =
					cmdHandler->getSolver()->stepBackward(count);
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
	}
};

}

#endif /* SHELL_STEPBACKWARDCOMMAND_HPP_ */
