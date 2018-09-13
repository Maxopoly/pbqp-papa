#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PBQPSolutionTests
#include <boost/test/unit_test.hpp>

#include "graph/PBQPSolution.hpp"

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(simpleSetGet) {
	const int size = 50;
	PBQPSolution<int> sol = PBQPSolution<int>(size);
	for(int i = 0; i < size; i++) {
		sol.setSolution(i, i * 3);
	}
	for(int i = 0; i < size; i++) {
		BOOST_CHECK_EQUAL(sol.getSolution(i), i * 3);
	}
}

}
