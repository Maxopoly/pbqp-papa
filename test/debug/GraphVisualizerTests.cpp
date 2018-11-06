#ifndef DEBUG_GRAPHVISUALIZERTESTS_CPP_
#define DEBUG_GRAPHVISUALIZERTESTS_CPP_

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GraphVisualizerTests
#include <boost/test/unit_test.hpp>
#include <algorithm>

#include "graph/PBQPGraph.hpp"
#include "util/TestUtils.hpp"
#include "debug/GraphVisualizer.hpp"
#include "io/PBQP_Serializer.hpp"

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(basic) {
	PBQP_Serializer<InfinityWrapper<unsigned int>> serial;
	PBQPGraph<InfinityWrapper<unsigned int>>* graph = serial.loadFromFile("test/testData/smallLibfirmGraph.json");
	dump<InfinityWrapper<unsigned int>>(graph, "mintput.svg");
}

BOOST_AUTO_TEST_CASE(loaded) {
	PBQP_Serializer<InfinityWrapper<unsigned int>> serial;
	PBQPGraph<InfinityWrapper<unsigned int>>* graph = serial.loadFromFile("test/testData/normalLibfirmGraph.json");
	dump<InfinityWrapper<unsigned int>>(graph, "basicOutput.svg");
}


}

#endif /* DEBUG_GRAPHVISUALIZERTESTS_CPP_ */
