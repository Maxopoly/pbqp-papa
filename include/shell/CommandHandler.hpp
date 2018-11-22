#ifndef SHELL_COMMANDHANDLER_HPP_
#define SHELL_COMMANDHANDLER_HPP_

#include <string>
#include <map>

#include "shell/Command.hpp"
#include "shell/DumpCommand.hpp"
#include "shell/StepForwardCommand.hpp"
#include "shell/StepBackwardCommand.hpp"
#include "shell/InfoCommand.hpp"

namespace pbqppapa {

template<typename T>
class Command;
template<typename T>
class StepByStepSolver;
template<typename T>
class DumpCommand;
template<typename T>
class StepForwardCommand;
template<typename T>
class StepBackwardCommand;
template<typename T>
class InfoCommand;

template<typename T>
class CommandHandler {
private:

	std::map<std::string, Command<T>> commands;
	std::string error = "Unknown command: ";
	StepByStepSolver<T> solver;

public:

	CommandHandler() {
		registerCommand(DumpCommand<T>());
		registerCommand(StepForwardCommand<T>());
		registerCommand(StepBackwardCommand<T>());
		registerCommand(InfoCommand<T>());
	}

	std::string execute(std::string input) {
		size_t spaceIndex = input.find(" ");
		std::string commandString;
		std::string commandArgs;
		if (spaceIndex == input.npos) {
			//not found, so take the entire string
			commandString = input;
		}
		else {
			commandString = input.substr(0, spaceIndex);
			commandArgs = input.substr(spaceIndex + 1);
		}
		auto iter = commands.find(commandString);
		if (iter == commands.end()) {
			//command doesnt exist
			return error + input;
		}
		Command<T> command = iter->second;
		return command.run(commandArgs);
	}

	void registerCommand(Command<T> command) {
		commands.insert(std::pair<std::string, Command<T>>(command.getIdentifier(), command));
	}
};

}



#endif /* SHELL_COMMANDHANDLER_HPP_ */
