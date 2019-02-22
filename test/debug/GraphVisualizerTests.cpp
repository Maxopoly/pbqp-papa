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
#if PBQP_USE_GVC
	PBQP_Serializer<InfinityWrapper<unsigned int>> serial;
	PBQPGraph<InfinityWrapper<unsigned int>>* graph = serial.loadFromFile(
			"test/testData/smallLibfirmGraph.json");
	GraphVisualizer<InfinityWrapper<unsigned int>>::dump(graph, "test_build/mintput.svg");
}

BOOST_AUTO_TEST_CASE(loaded) {
	PBQP_Serializer<InfinityWrapper<unsigned int>> serial;
	PBQPGraph<InfinityWrapper<unsigned int>>* graph = serial.loadFromFile(
			"test/testData/normalLibfirmGraph.json");
	GraphVisualizer<InfinityWrapper<unsigned int>>::dump(graph, "test_build/basicOutput.svg");
}

BOOST_AUTO_TEST_CASE(manual) {
	PBQPGraph<InfinityWrapper<unsigned int>>* graph = new PBQPGraph<
			InfinityWrapper<unsigned int>>();
	InfinityWrapper<unsigned int> arr[2];
	arr[0] = InfinityWrapper<unsigned int>(2);
	arr[1] = InfinityWrapper<unsigned int>(3);
	Vector<InfinityWrapper<unsigned int>> vector(2, arr);
	PBQPNode<InfinityWrapper<unsigned int>>* node = graph->addNode(vector);
	InfinityWrapper<unsigned int> arr2[2];
	arr2[0] = InfinityWrapper<unsigned int>(2);
	arr2[1] = InfinityWrapper<unsigned int>(1);
	Vector<InfinityWrapper<unsigned int>> vector2(2, arr2);
	PBQPNode<InfinityWrapper<unsigned int>>* node2 = graph->addNode(vector2);
	InfinityWrapper<unsigned int> arr3[4];
	arr3[0] = InfinityWrapper<unsigned int>(2);
	arr3[1] = InfinityWrapper<unsigned int>(1);
	arr3[2] = InfinityWrapper<unsigned int>(0);
	arr3[3] = InfinityWrapper<unsigned int>(1);
	Matrix<InfinityWrapper<unsigned int>> mat(2, 2, arr3);
	graph->addEdge(node, node2, mat);
	GraphVisualizer<InfinityWrapper<unsigned int>>::dump(graph, "test_build/the.svg", true);
#endif
}

}

#endif /* DEBUG_GRAPHVISUALIZERTESTS_CPP_ */
