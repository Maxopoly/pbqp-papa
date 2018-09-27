#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DegreeTwoReductorTests
#include <boost/test/unit_test.hpp>

#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "reduction/PBQPReduction.hpp"
#include "reduction/degree/DegreeTwoReductor.hpp"
#include "graph/PBQPSolution.hpp"

#include "util/TestUtils.hpp"

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(emptyGraphTest) {
	//make sure this doesnt explode
	PBQPGraph<int> graph;
	DegreeTwoReductor<int> twoReductor(&graph);
	std::vector<PBQPGraph<int>*> result = twoReductor.reduce();
}

BOOST_AUTO_TEST_CASE(simpleCalculation) {
	PBQPGraph<int> graph = PBQPGraph<int>();
	int node1Arr[] { 1, 2 };
	int node2Arr[] { 5, 3 };
	int node3Arr[] { 10, 2 };
	Vector<int> vek1(2, node1Arr);
	Vector<int> vek2(2, node2Arr);
	Vector<int> vek3(2, node3Arr);
	PBQPNode<int>* first = graph.addNode(vek1);
	PBQPNode<int>* second = graph.addNode(vek2);
	PBQPNode<int>* third = graph.addNode(vek3);
	unsigned long firstIndex = first->getIndex();
	unsigned long secondIndex = second->getIndex();
	unsigned long thirdIndex = third->getIndex();
	int edge1Arr[] { 3, 1, 8, 5 };
	Matrix<int> mat1(2, 2, edge1Arr);
	int edge2Arr[] { 0, 7, 2, 8 };
	Matrix<int> mat2(2, 2, edge2Arr);
	graph.addEdge(first, second, mat1);
	graph.addEdge(second, third, mat2);
	//second one gets removed
	DegreeTwoReductor<int> twoReductor(&graph);
	std::vector<PBQPGraph<int>*> result = twoReductor.reduce();
	BOOST_CHECK_EQUAL(result.size(), 1);
	PBQPGraph<int>* resultGraph = result[0];
	BOOST_CHECK_EQUAL(resultGraph->getNodeCount(), 2);
	BOOST_CHECK_EQUAL(resultGraph->getEdgeCount(), 1);
	PBQPEdge<int>* edge = *(resultGraph->getEdgeBegin());
	BOOST_CHECK_EQUAL(edge->getMatrix().get(0, 0), 6);
	BOOST_CHECK_EQUAL(edge->getMatrix().get(0, 1), 12);
	BOOST_CHECK_EQUAL(edge->getMatrix().get(1, 0), 10);
	BOOST_CHECK_EQUAL(edge->getMatrix().get(1, 1), 16);
	PBQPSolution<int> sol(3);
	sol.setSolution(firstIndex, 0);
	sol.setSolution(thirdIndex, 0);
	twoReductor.solve(sol);
	BOOST_CHECK_EQUAL(sol.getSolution(secondIndex), 1);
}

}
