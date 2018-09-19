#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BruteForceSolverTests
#include <boost/test/unit_test.hpp>

#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "analysis/PBQPHandler.hpp"
#include "solve/PBQPSolver.hpp"
#include "solve/BruteForceSolver.hpp"
#include "graph/PBQPSolution.hpp"


#include "util/TestUtils.hpp"

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(emptyGraphTest) {
	//make sure this doesnt explode
	PBQPGraph<int> graph;
	BruteForceSolver<int> solver(&graph);
	PBQPSolution<int>* sol = solver.calcSolution();
	delete sol;
}

BOOST_AUTO_TEST_CASE(simpleNodeNoEdgesTest) {
	PBQPGraph<int> graph;
	int arr [] { 3, 1, 2};
	Vector<int> vek (3, arr);
	PBQPNode<int>* node1 = graph.addNode(vek);
	int arr2 [] { 3, 5, 2, 1, 4};
	Vector<int> vek2 (5, arr2);
	PBQPNode<int>* node2 = graph.addNode(vek2);
	int arr3 [] { 10, 9, 0, 2, 4};
	Vector<int> vek3 (5, arr3);
	PBQPNode<int>* node3 = graph.addNode(vek3);
	BruteForceSolver<int> solver(&graph);
	PBQPSolution<int>* sol = solver.calcSolution();
	BOOST_CHECK_EQUAL(sol->getSolution(node1->getIndex()), 1);
	BOOST_CHECK_EQUAL(sol->getSolution(node2->getIndex()), 3);
	BOOST_CHECK_EQUAL(sol->getSolution(node3->getIndex()), 2);
	delete sol;
}

BOOST_AUTO_TEST_CASE(simpleEdge) {
	PBQPGraph<int> graph;
	int arr [] { 3, 1, 1};
	Vector<int> vek (3, arr);
	PBQPNode<int>* node1 = graph.addNode(vek);
	int arr2 [] { 3, 5, 3};
	Vector<int> vek2 (3, arr2);
	PBQPNode<int>* node2 = graph.addNode(vek2);
	int matArr [] {20, 31, 20, 20, 22, 10, 24, 25, 26};
	Matrix<int> mat (3, 3, matArr);
	graph.addEdge(node1, node2, mat);
	BruteForceSolver<int> solver(&graph);
	PBQPSolution<int>* sol = solver.calcSolution();
	BOOST_CHECK_EQUAL(sol->getSolution(node1->getIndex()), 1);
	BOOST_CHECK_EQUAL(sol->getSolution(node2->getIndex()), 2);
	delete sol;
}

}
