#ifndef SHELL_INFOCOMMAND_HPP_
#define SHELL_INFOCOMMAND_HPP_

#include <string>

#include "shell/Command.hpp"

namespace pbqppapa {

template<typename T>
class Command;
template<typename T>
class PBQPGraph;
template<typename T>
class CommandHandler;

template<typename T>
class InfoCommand : public Command<T> {

public:
	InfoCommand() :
		Command<T>("info") {
	}

	 ~InfoCommand() {
	}

	 std::string run(std::string input, CommandHandler<T>* cmdHandler) override {
		 PBQPGraph<InfinityWrapper<T>>* graph = cmdHandler->getSolver()->getGraph();
		 std::string result;
		 result += "Graph has ";
		 result += std::to_string(graph->getNodeCount());
		 result += " nodes and ";
		 result += std::to_string(graph->getEdgeCount());
		 result += " edges";
		 return result;
	 }
};

}



#endif /* SHELL_INFOCOMMAND_HPP_ */
