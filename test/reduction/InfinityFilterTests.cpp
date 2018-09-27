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
#include "reduction/degree/DegreeZeroReducer.hpp"
#include "graph/PBQPSolution.hpp"
#include "io/PBQP_Serializer.hpp"

#include "util/TestUtils.hpp"

namespace pbqppapa {


BOOST_AUTO_TEST_CASE(reduction) {
	PBQP_Serializer<unsigned long> serial ();



	PBQPGraph<unsigned long> graph = PBQPGraph<unsigned long>();
	for (int i = 0; i < 20; i++) {
		unsigned long arr[] { 3, 1 };
		Vector<unsigned long> vek (2, arr);
		PBQPNode<unsigned long>* node = graph.addNode(vek);
	}
	DegreeZeroReducer<unsigned long> zeroReducer(&graph);
	std::vector<PBQPGraph<unsigned long>*> result = zeroReducer.reduce();
	BOOST_CHECK_EQUAL(result.size(), 1);
	PBQPGraph<unsigned long>* resultGraph = result [0];
	BOOST_CHECK_EQUAL(resultGraph->getNodeCount(), 0);
}


}
