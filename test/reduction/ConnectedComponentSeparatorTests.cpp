#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ConnectedComponentSeparatorTests
#include <boost/test/unit_test.hpp>

#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "reduction/PBQPReduction.hpp"
#include "reduction/ConnectedComponentSeparator.hpp"
#include "graph/PBQPSolution.hpp"

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(singleNodeTest) {
	PBQPGraph<int> graph = PBQPGraph<int>();
	int vekData [] = {2 ,2};
	Vektor<int> vek = Vektor<int>(2, vekData);
	graph.addNode(vek);
	ConnectedComponentSeparator<int> sep = ConnectedComponentSeparator<int>(&graph);
	std::vector<PBQPGraph<int>*> components = sep.reduce();
	PBQPSolution<int> sol = PBQPSolution<int>(0);
	sep.solve(sol);
	BOOST_CHECK_EQUAL(components.size(), 1);
	PBQPGraph<int>* retrievedGraph = components[0];
	BOOST_CHECK_EQUAL(0, retrievedGraph->getEdgeCount());
	BOOST_CHECK_EQUAL(1, retrievedGraph->getNodeCount());
	BOOST_CHECK_EQUAL((*(graph.getNodeBegin()))->getIndex(),
			(*(retrievedGraph->getNodeBegin()))->getIndex());
	if (retrievedGraph != &graph) {
		//not neccessary for our implementation, but just to make sure
		delete retrievedGraph;
	}
}

BOOST_AUTO_TEST_CASE(emptyGraphTest) {
	PBQPGraph<int> graph = PBQPGraph<int>();
	ConnectedComponentSeparator<int> sep = ConnectedComponentSeparator<int>(&graph);
	std::vector<PBQPGraph<int>*> components = sep.reduce();
	PBQPSolution<int> sol = PBQPSolution<int>(0);
	sep.solve(sol);
	BOOST_CHECK_EQUAL(components.size(), 1);
	PBQPGraph<int>* retrievedGraph = components[0];
	BOOST_CHECK_EQUAL(0, retrievedGraph->getEdgeCount());
	BOOST_CHECK_EQUAL(0, retrievedGraph->getNodeCount());
}

BOOST_AUTO_TEST_CASE(basicNodeTest) {
	PBQPGraph<int> graph = PBQPGraph<int>();
	int size = 50;
	for (int i = 0; i < size; i++) {
		int arr [] = {2, 2};
		Vektor<int> vek = Vektor<int>(2, arr);
		graph.addNode(vek);
	}
	ConnectedComponentSeparator<int> sep = ConnectedComponentSeparator<int>(&graph);
	std::vector<PBQPGraph<int>*> components = sep.reduce();
	PBQPSolution<int> sol = PBQPSolution<int>(0);
	sep.solve(sol);
	BOOST_CHECK_EQUAL(components.size(), size);
	std::set<int> nodeIndices = std::set<int>();
	for (int i = 0; i < size; i++) {
		PBQPGraph<int>* retrievedGraph = components[i];
		BOOST_CHECK_EQUAL(0, retrievedGraph->getEdgeCount());
		BOOST_CHECK_EQUAL(1, retrievedGraph->getNodeCount());
		unsigned int index = (*(retrievedGraph->getNodeBegin()))->getIndex();
		BOOST_CHECK_EQUAL(0, nodeIndices.count(index));
		nodeIndices.insert(index);
		if (retrievedGraph != &graph) {
			delete retrievedGraph;
		}
	}
}

BOOST_AUTO_TEST_CASE(advancedNodeTest) {
	PBQPGraph<int>* graph = new PBQPGraph<int>();
	int subgraphs = 10;
	int localSize = 10;
	int edgeCount = 0;
	for (int i = 0; i < subgraphs; i++) {
		edgeCount = 0;
		std::vector<PBQPNode<int>*> otherNodes = std::vector<PBQPNode<int>*>();
		for (int k = 0; k < localSize; k++) {
			int arr [] = {2, 2};
			Vektor<int> vek = Vektor<int>(2, arr);
			PBQPNode<int>* node = graph->addNode(vek);
			otherNodes.push_back(node);
			for (PBQPNode<int>* otherNode : otherNodes) {
				int arr2 [] = {3, 2, 5, 8};
				Matrix<int> mat = Matrix<int>(2, 2, arr2);
				graph->addEdge(node, otherNode, mat);
				edgeCount++;
			}
		}
	}
	ConnectedComponentSeparator<int> sep = ConnectedComponentSeparator<int>(graph);
	std::vector<PBQPGraph<int>*> components = sep.reduce();
	PBQPSolution<int> sol = PBQPSolution<int>(0);
	sep.solve(sol);
	BOOST_CHECK_EQUAL(components.size(), subgraphs);
	std::set<int> nodeIndices = std::set<int>();
	for (int i = 0; i < subgraphs; i++) {
		PBQPGraph<int>* retrievedGraph = components[i];
		BOOST_CHECK_EQUAL(edgeCount, retrievedGraph->getEdgeCount());
		BOOST_CHECK_EQUAL(localSize, retrievedGraph->getNodeCount());
		//ensure each node is only in one subgraph
		for (auto iter = retrievedGraph->getNodeBegin();
				iter != retrievedGraph->getNodeEnd(); ++iter) {
			unsigned int index = (*iter)->getIndex();
			BOOST_CHECK_EQUAL(0, nodeIndices.count(index));
			nodeIndices.insert(index);
		}
	}
	for (PBQPGraph<int>* subGraph : components) {
		delete subGraph;
	}
}

}

