#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SelfCycleRemoverTests
#include <boost/test/unit_test.hpp>

#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "reduction/PBQPReduction.hpp"
#include "reduction/SelfCycleRemover.hpp"
#include "graph/PBQPSolution.hpp"

#include "util/TestUtils.hpp"

BOOST_AUTO_TEST_CASE(emptyGraphTest) {
	//make sure this doesnt explode
	PBQPGraph<int> graph = *new PBQPGraph<int>();
	SelfCycleRemover<int> cycleRemover = *new SelfCycleRemover<int>(&graph);
	std::vector<PBQPGraph<int>*> result = *(cycleRemover.reduce());
}

BOOST_AUTO_TEST_CASE(simpleKnTest) {
	int size = 30;
	PBQPGraph<int>* graph = genGraph(size);
	SelfCycleRemover<int> cycleRemover = *new SelfCycleRemover<int>(graph);
	std::vector<PBQPGraph<int>*> result = *(cycleRemover.reduce());
	BOOST_CHECK_EQUAL(result.size(), 1);
	if (graph != result[0]) {
		delete graph;
		graph = result[0];
	}
	BOOST_CHECK_EQUAL(graph->getEdgeCount(), size * (size - 1));
	BOOST_CHECK_EQUAL(graph->getNodeCount(), size);
	for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
			++iter) {
		PBQPNode<int>* node = *iter;
		BOOST_CHECK_EQUAL((size - 1) * 2, node->getDegree());
		BOOST_CHECK_EQUAL(node->getVektor()->get(0), 6);
		BOOST_CHECK_EQUAL(node->getVektor()->get(1), 10);
	}
	delete graph;
}

BOOST_AUTO_TEST_CASE(simpleNumberTest) {
	PBQPGraph<int>* graph = new PBQPGraph<int>();
	Vektor<int>* vek1 = new Vektor<int>(3, new int[3] { 1, 2, 3 });
	PBQPNode<int>* node1 = graph->addNode(vek1);
	Vektor<int>* vek2 = new Vektor<int>(2, new int[2] { 4, 5 });
	PBQPNode<int>* node2 = graph->addNode(vek2);
	Vektor<int>* vek3 = new Vektor<int>(3, new int[3] { 6, 7, 8 });
	PBQPNode<int>* node3 = graph->addNode(vek3);
	//normal edges
	graph->addEdge(node1, node2, new Matrix<int>(3, 2, new int[6] { 5, 5, 5, 5,
			5, 5 }));
	graph->addEdge(node2, node3, new Matrix<int>(2, 3, new int[6] { 5, 5, 5, 5,
			5, 5 }));
	graph->addEdge(node3, node1, new Matrix<int>(3, 3, new int[9] { 5, 5, 5, 5,
			5, 5, 5, 5, 5 }));
	//cycles
	graph->addEdge(node1, node1, new Matrix<int>(3, 3, new int[9] { 9, 10, 11,
			12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1, new Matrix<int>(3, 3, new int[9] { 9, 10, 11,
			12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1, new Matrix<int>(3, 3, new int[9] { 9, 10, 11,
			12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node2, node2, new Matrix<int>(2, 2, new int[4] { 20, 21, 22,
			23 }));
	SelfCycleRemover<int> cycleRemover = *new SelfCycleRemover<int>(graph);
	std::vector<PBQPGraph<int>*> result = *(cycleRemover.reduce());
	BOOST_CHECK_EQUAL(result.size(), 1);
	if (graph != result[0]) {
		delete graph;
		graph = result[0];
	}
	BOOST_CHECK_EQUAL(node1->getDegree(), 2);
	BOOST_CHECK_EQUAL(node1->getVektor()->get(0), 28);
	BOOST_CHECK_EQUAL(node1->getVektor()->get(1), 41);
	BOOST_CHECK_EQUAL(node1->getVektor()->get(2), 54);
	BOOST_CHECK_EQUAL(node2->getDegree(), 2);
	BOOST_CHECK_EQUAL(node2->getVektor()->get(0), 24);
	BOOST_CHECK_EQUAL(node2->getVektor()->get(1), 28);
	BOOST_CHECK_EQUAL(node3->getDegree(), 2);
	BOOST_CHECK_EQUAL(node3->getVektor()->get(0), 6);
	BOOST_CHECK_EQUAL(node3->getVektor()->get(1), 7);
	BOOST_CHECK_EQUAL(node3->getVektor()->get(2), 8);
	delete graph;
}

