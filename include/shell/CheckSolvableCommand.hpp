#ifndef SHELL_CHECKSOLVABLECOMMAND_HPP_
#define SHELL_CHECKSOLVABLECOMMAND_HPP_

#include "shell/Command.hpp"
#include "io/TypeSerializers.hpp"

namespace pbqppapa {

template<typename T>
class Command;
template<typename T>
class CommandHandler;
template<typename T>
class PBQP_Serializer;
template<typename T>
class InfinityWrapper;
template<typename T>
class TypeSerializers;

template<typename T>
class CheckSolvableCommand: public Command<T> {

public:
	CheckSolvableCommand() :
			Command<T>("check") {
	}

	~CheckSolvableCommand() {
	}

	std::string run(std::string input, CommandHandler<T>* cmdHandler) {
		if (cmdHandler->getSolver()->isSolvable()) {
			return "Solution exists";
		}
		return "PEO did not find a valid solution";
	}

};

}

#endif /* SHELL_CHECKSOLVABLECOMMAND_HPP_ */
