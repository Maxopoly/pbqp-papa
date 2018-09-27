#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DegreeZeroReducerTests
#include <boost/test/unit_test.hpp>

#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "reduction/PBQPReduction.hpp"
#include "reduction/degree/DegreeZeroReducer.hpp"
#include "graph/PBQPSolution.hpp"

#include "util/TestUtils.hpp"

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(emptyGraphTest) {
	//make sure this doesnt explode
	PBQPGraph<int> graph;
	DegreeZeroReducer<int> zeroReducer (&graph);
	std::vector<PBQPGraph<int>*> result = zeroReducer.reduce();
}

BOOST_AUTO_TEST_CASE(simpleNodeReduction) {
	PBQPGraph<int> graph = PBQPGraph<int>();
	for (int i = 0; i < 20; i++) {
		int arr[] { 3, 1 };
		Vector<int> vek (2, arr);
		PBQPNode<int>* node = graph.addNode(vek);
	}
	DegreeZeroReducer<int> zeroReducer(&graph);
	std::vector<PBQPGraph<int>*> result = zeroReducer.reduce();
	BOOST_CHECK_EQUAL(result.size(), 1);
	PBQPGraph<int>* resultGraph = result [0];
	BOOST_CHECK_EQUAL(resultGraph->getNodeCount(), 0);
}

}
