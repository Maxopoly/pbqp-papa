#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ConnectedComponentSeparatorTests
#include <boost/test/unit_test.hpp>

#include <vector>

#include "graph/PBQP_Graph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"
#include "reduction/PBQPReduction.hpp"
#include "reduction/NodeConsistentReduction.hpp"
#include "reduction/ConnectedComponentSeparator.hpp"
#include "graph/PBQP_Solution.hpp"

BOOST_AUTO_TEST_CASE(singleNodeTest) {
	PBQP_Graph<int> graph = *(new PBQP_Graph<int>());
	graph.addNode(new Vektor<int>(2, new int[2] { 2, 2 }));
	ConnectedComponentSeparator<int> sep = *(new ConnectedComponentSeparator<int>(
			&graph));
	std::vector<PBQP_Graph<int>*> components = *(sep.reduce());
	PBQP_Solution<int> sol = *(new PBQP_Solution<int>(0));
	sep.solve(&sol);
	BOOST_CHECK_EQUAL(components.size(), 1);
	PBQP_Graph<int> retrievedGraph = *(components [0]);
	BOOST_CHECK_EQUAL(graph.getEdgeCount(), retrievedGraph.getEdgeCount());
	BOOST_CHECK_EQUAL(graph.getNodeCount(), retrievedGraph.getNodeCount());
	BOOST_CHECK_EQUAL((*(graph.getNodes()->begin()))->getIndex(), (*(retrievedGraph.getNodes()->begin()))->getIndex());
}

BOOST_AUTO_TEST_CASE(emptyGraphTest) {
	PBQP_Graph<int>* graph = new PBQP_Graph<int>();
	ConnectedComponentSeparator<int> sep = *new ConnectedComponentSeparator<int>(
			graph);
	sep.reduce();
	PBQP_Solution<int>* sol = new PBQP_Solution<int>(0);
	sep.solve(sol);
}
