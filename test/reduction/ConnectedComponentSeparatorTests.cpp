#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ConnectedComponentSeparatorTests
#include <boost/test/unit_test.hpp>

#include <vector>
#include <set>

#include "graph/PBQP_Graph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"
#include "reduction/PBQPReduction.hpp"
#include "reduction/ConnectedComponentSeparator.hpp"
#include "graph/PBQP_Solution.hpp"

BOOST_AUTO_TEST_CASE(singleNodeTest) {
	PBQP_Graph<int> graph = *(new PBQP_Graph<int>());
	graph.addNode(new Vektor<int>(2, new int[2] { 2, 2 }));
	ConnectedComponentSeparator<int> sep =
			*(new ConnectedComponentSeparator<int>(&graph));
	std::vector<PBQP_Graph<int>*> components = *(sep.reduce());
	PBQP_Solution<int> sol = *(new PBQP_Solution<int>(0));
	sep.solve(&sol);
	BOOST_CHECK_EQUAL(components.size(), 1);
	PBQP_Graph<int>* retrievedGraph = components[0];
	BOOST_CHECK_EQUAL(0, retrievedGraph->getEdgeCount());
	BOOST_CHECK_EQUAL(1, retrievedGraph->getNodeCount());
	BOOST_CHECK_EQUAL((*(graph.getNodes()->begin()))->getIndex(),
			(*(retrievedGraph->getNodes()->begin()))->getIndex());
	if (retrievedGraph != &graph) {
		//not neccessary for our implementation, but just to make sure
		delete retrievedGraph;
	}
}

BOOST_AUTO_TEST_CASE(emptyGraphTest) {
	PBQP_Graph<int> graph = *(new PBQP_Graph<int>());
	ConnectedComponentSeparator<int> sep =
			*new ConnectedComponentSeparator<int>(&graph);
	std::vector<PBQP_Graph<int>*> components = *(sep.reduce());
	PBQP_Solution<int> sol = *(new PBQP_Solution<int>(0));
	sep.solve(&sol);
	BOOST_CHECK_EQUAL(components.size(), 1);
	PBQP_Graph<int>* retrievedGraph = components[0];
	BOOST_CHECK_EQUAL(0, retrievedGraph->getEdgeCount());
	BOOST_CHECK_EQUAL(0, retrievedGraph->getNodeCount());
}

BOOST_AUTO_TEST_CASE(basicNodeTest) {
	PBQP_Graph<int> graph = *(new PBQP_Graph<int>());
	int size = 50;
	for (int i = 0; i < size; i++) {
		graph.addNode(new Vektor<int>(2, new int[2] { 2, 2 }));
	}
	ConnectedComponentSeparator<int> sep =
			*(new ConnectedComponentSeparator<int>(&graph));
	std::vector<PBQP_Graph<int>*> components = *(sep.reduce());
	PBQP_Solution<int> sol = *(new PBQP_Solution<int>(0));
	sep.solve(&sol);
	BOOST_CHECK_EQUAL(components.size(), size);
	std::set<int> nodeIndices = *new std::set<int>();
	for (int i = 0; i < size; i++) {
		PBQP_Graph<int>* retrievedGraph = components[i];
		BOOST_CHECK_EQUAL(0, retrievedGraph->getEdgeCount());
		BOOST_CHECK_EQUAL(1, retrievedGraph->getNodeCount());
		unsigned int index = (*(retrievedGraph->getNodes()->begin()))->getIndex();
		BOOST_CHECK_EQUAL(0, nodeIndices.count(index));
		nodeIndices.insert(index);
		if (retrievedGraph != &graph) {
			delete retrievedGraph;
		}
	}
}
