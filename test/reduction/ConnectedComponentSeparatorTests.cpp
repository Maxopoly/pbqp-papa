#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ConnectedComponentSeparatorTests
#include <boost/test/unit_test.hpp>

#include "graph/PBQP_Graph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"
#include "reduction/ConnectedComponentSeparator.hpp"
#include "graph/PBQP_Solution.hpp"

BOOST_AUTO_TEST_CASE(singleNodeTest) {
	PBQP_Graph<int>* graph = new PBQP_Graph<int>();
	graph->addNode(new Vektor<int>(2, new int[2] { 2, 2 }));


}

BOOST_AUTO_TEST_CASE(emptyGraphTest) {
	PBQP_Graph<int>* graph = new PBQP_Graph<int>();
	ConnectedComponentSeparator<int> sep = *new ConnectedComponentSeparator<int>(
			graph);
	sep.reduce();
	sep.solve(new PBQP_Solution<int>(0));
}
