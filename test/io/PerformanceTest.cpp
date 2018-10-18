#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PerformanceTest
#include <boost/test/unit_test.hpp>
#include <float.h>
#include <algorithm>

#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "io/PBQP_Serializer.hpp"
#include "generate/PBQPGenerator.hpp"
#include "analysis/SolutionAmountChecker.hpp"
#include "FullSolver.hpp"
#include "CInterface.h"

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(libTest) {
	PBQP_Serializer<InfinityWrapper<unsigned int>> serial;
	std::vector<std::string> paths;
	paths.push_back("test/testData/giantLibfirmGraph.json");
	paths.push_back("test/testData/bigLibfirmGraph.json");
	paths.push_back("test/testData/normalLibfirmGraph.json");
	paths.push_back("test/testData/smallLibfirmGraph.json");
	int testRuns = 20;
	std::cout << "Running reduction " << std::to_string(testRuns) << " times\n";
	for (std::string path : paths) {
		DebugTimer timer;
		double sum = 0;
		double mini = DBL_MAX;
		double maxi = 0;
		PBQPGraph<InfinityWrapper<unsigned int>>* ogGraph = serial.loadFromFile(
				path);
		StatKeeper stats;
		stats.submitGraphBefore(ogGraph);
		std::cout << "Stats for graph " << path << " "
				<< stats.getGraphDescription();
		for (int i = 0; i < testRuns; i++) {
			PBQPGraph<InfinityWrapper<unsigned int>>* graph = new PBQPGraph<
					InfinityWrapper<unsigned int>>(ogGraph);
			timer.startTimer();
			FullSolver<InfinityWrapper<unsigned int>> solver(graph);
			PBQPSolution<InfinityWrapper<unsigned int>>* sol = solver.solve();
			timer.stopTimer();
			delete sol;
			double duration = timer.getDuration();
			sum += duration;
			maxi = std::max(maxi, duration);
			mini = std::min(mini, duration);
			delete graph;
		}
		std::cout << "Graph " << path << " processed in avg: "
				<< std::to_string(sum / (double) testRuns) << ", min: "
				<< std::to_string(mini) << ", max: " << std::to_string(maxi)
				<< '\n';
	}
}

}
