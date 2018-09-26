#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE InfinityFilterTests
#include <boost/test/unit_test.hpp>

#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "reduction/InfinityFilter.hpp"
#include "reduction/degree/DegreeZeroReductor.hpp"
#include "graph/PBQPSolution.hpp"
#include "io/PBQP_Serializer.hpp"

#include "util/TestUtils.hpp"

namespace pbqppapa {


BOOST_AUTO_TEST_CASE(reduction) {
	PBQP_Serializer<unsigned long> serial ();



	PBQPGraph<unsigned long>* graph = PBQPGraph<int>();
	for (int i = 0; i < 20; i++) {
		int arr[] { 3, 1 };
		Vector<int> vek (2, arr);
		PBQPNode<int>* node = graph.addNode(vek);
	}
	DegreeZeroReductor<int> zeroReductor(&graph);
	std::vector<PBQPGraph<int>*> result = zeroReductor.reduce();
	BOOST_CHECK_EQUAL(result.size(), 1);
	PBQPGraph<int>* resultGraph = result [0];
	BOOST_CHECK_EQUAL(resultGraph->getNodeCount(), 0);
}


}
