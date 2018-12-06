#ifndef DEBUG_SHELLTEST_CPP_
#define DEBUG_SHELLTEST_CPP_

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ShellTest
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <readline/readline.h>
#include "shell/CommandHandler.hpp"
#include "io/PBQP_Serializer.hpp"
#include "graph/PBQPGraph.hpp"
#include "math/InfinityWrapper.hpp"


namespace pbqppapa {

BOOST_AUTO_TEST_CASE(basic) {
	PBQP_Serializer<InfinityWrapper<unsigned int>> serial;
	PBQPGraph<InfinityWrapper<unsigned int>>* graph = serial.loadFromFile(
			"test/testData/problematic.json");
	CommandHandler<unsigned int> solver(graph);
	while (true) {
		char *buffer = readline("> ");
		if (buffer) {
			std::cout << solver.execute(std::string(buffer)) << '\n';
		}
		delete buffer;
	}
}
}

#endif /* DEBUG_SHELLTEST_CPP_ */
