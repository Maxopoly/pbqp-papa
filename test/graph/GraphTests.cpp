#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GraphTests
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <algorithm>

#include "graph/PBQPGraph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"

#include "util/TestUtils.hpp"

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(basicEdgeGeneration) {
	PBQPGraph<int>* graph = new PBQPGraph<int>();
	BOOST_CHECK_EQUAL(graph->getEdgeCount(), 0);
	BOOST_CHECK_EQUAL(graph->getNodeCount(), 0);

	//generate a bunch of nodes
	for (int i = 1; i <= 50; i++) {
		int arr1 [] = { 3, 2 };
		Vektor<int> vektor = Vektor<int>(2, arr1);
		PBQPNode<int>* node = graph->addNode(vektor);
		BOOST_CHECK_EQUAL(graph->getEdgeCount(), 0);
		BOOST_CHECK_EQUAL(graph->getNodeCount(), i);
		BOOST_CHECK_EQUAL(node->getIndex(), i - 1);
		BOOST_CHECK_EQUAL(node->getVektor(), vektor);
		BOOST_CHECK_EQUAL(node->getDegree(), 0);
		BOOST_CHECK_EQUAL(node->getVektorDegree(), 2);
	}

	//generate a bunch of edges
	PBQPNode<int>* node1 = *(graph->getNodeBegin());
	int counter = 0;
	for (std::set<PBQPNode<int>*>::iterator it = graph->getNodeBegin();
			it != graph->getNodeEnd(); it++) {
		int arr1 [] = { 3, 2, 5, 8 };
		Matrix<int> matrix = Matrix<int>(2, 2, arr1);
		PBQPNode<int>* node2 = *it;
		BOOST_CHECK_EQUAL(0, node2->getDegree());
		std::vector<PBQPNode<int>*>* adjaNodes = node1->getAdjacentNodes(true);
		BOOST_CHECK(
				std::find(adjaNodes->begin(), adjaNodes->end(), node2)
						== adjaNodes->end());
		PBQPEdge<int>* edge = graph->addEdge(node1, node2, matrix);
		BOOST_CHECK_EQUAL(graph->getEdgeCount(), counter + 1);
		BOOST_CHECK_EQUAL(graph->getNodeCount(), 50);
		BOOST_CHECK_EQUAL(counter + 1, node1->getDegree());
		BOOST_CHECK_EQUAL(1, node2->getDegree());
		delete adjaNodes;
		adjaNodes = node1->getAdjacentNodes(true);
		BOOST_CHECK(
				std::find(adjaNodes->begin(), adjaNodes->end(), node2)
						!= adjaNodes->end());
		BOOST_CHECK_EQUAL(adjaNodes->size(), counter + 1);
		delete adjaNodes;
		adjaNodes = node1->getAdjacentNodes(false);
		BOOST_CHECK(
				std::find(adjaNodes->begin(), adjaNodes->end(), node2)
						!= adjaNodes->end());
		BOOST_CHECK_EQUAL(adjaNodes->size(), counter + 1);
		if (counter != 0) {
			//exclude initial cycle
			delete adjaNodes;
			adjaNodes = node2->getAdjacentNodes(true);
			BOOST_CHECK(
					std::find(adjaNodes->begin(), adjaNodes->end(), node1)
							== adjaNodes->end());
			BOOST_CHECK_EQUAL(adjaNodes->size(), 0);
		}
		delete adjaNodes;
		adjaNodes = node2->getAdjacentNodes(false);
		BOOST_CHECK(
				std::find(adjaNodes->begin(), adjaNodes->end(), node1)
						!= adjaNodes->end());
		BOOST_CHECK_EQUAL(adjaNodes->size(), 1);
		std::vector<PBQPEdge<int>*>* adjaEdge = node1->getAdjacentEdges(true);
		BOOST_CHECK(
				std::find(adjaEdge->begin(), adjaEdge->end(), edge)
						!= adjaEdge->end());
		BOOST_CHECK_EQUAL(adjaEdge->size(), counter + 1);
		counter++;
	}
	delete graph;
}

BOOST_AUTO_TEST_CASE(advancedEdgeGeneration) {
	int size = 20;
	PBQPGraph<int>* graph = genGraph(size);
	BOOST_CHECK_EQUAL(graph->getEdgeCount(), size * size);
	BOOST_CHECK_EQUAL(graph->getNodeCount(), size);
	for (std::set<PBQPNode<int>*>::iterator it = graph->getNodeBegin();
			it != graph->getNodeEnd(); it++) {
		PBQPNode<int>* node = *it;
		BOOST_CHECK_EQUAL(node->getDegree(), size * 2 - 1);
		std::vector<PBQPNode<int>*>* adjaNodes = node->getAdjacentNodes(true);
		BOOST_CHECK_EQUAL(adjaNodes->size(), size);
		delete adjaNodes;
		adjaNodes = node->getAdjacentNodes(false);
		BOOST_CHECK_EQUAL(node->getAdjacentNodes(false)->size(), size);
		delete adjaNodes;
		BOOST_CHECK_EQUAL(node->getAdjacentEdges(false).size(), size * 2 - 1);
		BOOST_CHECK_EQUAL(node->getAdjacentEdges(true).size(), size);
	}
	delete graph;
}

BOOST_AUTO_TEST_CASE(advancedEdgeRemoval) {
	int size = 20;
	PBQPGraph<int>* graph = genGraph(size);
	PBQPNode<int>* node = *(graph->getNodeBegin());
	int removed = 0;
	std::vector<PBQPNode<int>*>* adjaNodes;
	for (PBQPEdge<int>* edge : node->getAdjacentEdges(true)) {
		PBQPNode<int>* other = edge->getOtherEnd(node);
		adjaNodes = other->getAdjacentNodes(true);
		bool otherWay = !(edge->isCycle());
		graph->removeEdge(edge);
		removed++;
		BOOST_CHECK_EQUAL(graph->getEdgeCount(), size * size - removed);
		BOOST_CHECK_EQUAL(node->getDegree(), size * 2 - 1 - removed);
		BOOST_CHECK_EQUAL(other->getDegree(), size * 2 - 2);
		adjaNodes = node->getAdjacentNodes(true);
		BOOST_CHECK(
				std::find(adjaNodes->begin(), adjaNodes->end(), other)
						== adjaNodes->end());
		adjaNodes = other->getAdjacentNodes(true);
		BOOST_CHECK_EQUAL(otherWay,
				std::find(adjaNodes->begin(), adjaNodes->end(), node)
						!= adjaNodes->end());
	}
	delete graph;
}

}
