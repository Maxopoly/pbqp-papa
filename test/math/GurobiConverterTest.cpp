#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GurobiConverterTests
#include <boost/test/unit_test.hpp>

#include "math/GurobiConverter.hpp"
#include "io/PBQP_Serializer.hpp"
#include "debug/DebugTimer.hpp"
#include <string>

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(empty) {
	PBQP_Serializer<InfinityWrapper<unsigned int>> serial;
	std::vector<std::string> paths;
	paths.push_back("test/testData/smallLibfirmGraph.json");
	paths.push_back("test/testData/normalLibfirmGraph.json");
	paths.push_back("test/testData/bigLibfirmGraph.json");
	paths.push_back("test/testData/giantLibfirmGraph.json");
	/*
	std::cout << "Linear solving: " << '\n';
	for (std::string path : paths) {
		std::cout << "Processing " << path << '\n';
		DebugTimer timer("Total timer: " + path);
		PBQPGraph<InfinityWrapper<unsigned int>>* ogGraph = serial.loadFromFile(
				path);
		GurobiConverter<unsigned int> gConv;
		timer.startTimer();
		PBQPSolution<InfinityWrapper<unsigned int>>* solution =gConv.solveGurobiLinear(ogGraph);
		timer.stopTimer();
		std::cout << timer.getOutput()
				<< std::to_string(solution->getTotalCost(ogGraph).getValue())
				<< '\n';
		delete solution;
	}
	*/
	std::cout << "Quadratic solving: " << '\n';
	for (std::string path : paths) {
		std::cout << "Processing " << path << '\n';
		DebugTimer timer("Total timer: " + path);
		PBQPGraph<InfinityWrapper<unsigned int>>* ogGraph = serial.loadFromFile(
				path);
		GurobiConverter<unsigned int> gConv;
		timer.startTimer();
		PBQPSolution<InfinityWrapper<unsigned int>>* solution =
				gConv.solveGurobiQuadratic(ogGraph);
		timer.stopTimer();

		std::cout << timer.getOutput()
				<< std::to_string(solution->getTotalCost(ogGraph).getValue())
				<< '\n';
		delete solution;
	}
}
}

