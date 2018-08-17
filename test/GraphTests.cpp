#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GraphTests
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <algorithm>

#include "graph/PBQP_Graph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"

BOOST_AUTO_TEST_CASE(basicGraphTests) {
	PBQP_Graph<int>* graph = new PBQP_Graph<int>();
	BOOST_CHECK_EQUAL(graph->getEdgeCount(), 0);
	BOOST_CHECK_EQUAL(graph->getNodeCount(), 0);

	//generate a bunch of nodes
	for (int i = 1; i <= 50; i++) {
		Vektor<int>* vektor = new Vektor<int>(2, new int[2] { 3, 2 });
		PBQP_Node<int>* node = graph->addNode(vektor);
		BOOST_CHECK_EQUAL(graph->getEdgeCount(), 0);
		BOOST_CHECK_EQUAL(graph->getNodeCount(), i);
		BOOST_CHECK_EQUAL(node->getIndex(), i - 1);
		BOOST_CHECK_EQUAL(node->getVektor(), vektor);
		BOOST_CHECK_EQUAL(node->getDegree(), 0);
		BOOST_CHECK_EQUAL(node->getVektorDegree(), 2);
	}

	//generate a bunch of edges
	PBQP_Node<int>* node1 = *(graph->getNodes()->begin());
	std::set<PBQP_Node<int>*>* nodes = graph->getNodes();
	int counter = 0;
	for (std::set<PBQP_Node<int>*>::iterator it = nodes->begin(); it != nodes->end(); it++) {
		Matrix<int>* matrix = new Matrix<int>(2, 2, new int[4] { 3, 2, 5, 8 });
		PBQP_Node<int>* node2 = *it;
		BOOST_CHECK_EQUAL(0, node2->getDegree());
		std::vector<PBQP_Node<int>*>* adjaNodes = node1->getAdjacentNodes(true);
		BOOST_CHECK(
				std::find(adjaNodes->begin(), adjaNodes->end(), node2)
						== adjaNodes->end());
		PBQP_Edge<int>* edge = graph->addEdge(node1, node2, matrix);
		BOOST_CHECK_EQUAL(graph->getEdgeCount(), counter + 1);
		BOOST_CHECK_EQUAL(graph->getNodeCount(), 50);
		BOOST_CHECK_EQUAL(counter + 1, node1->getDegree());
		BOOST_CHECK_EQUAL(1, node2->getDegree());

		adjaNodes = node1->getAdjacentNodes(true);
		BOOST_CHECK(
				std::find(adjaNodes->begin(), adjaNodes->end(), node2)
						!= adjaNodes->end());
		adjaNodes = node1->getAdjacentNodes(false);
		BOOST_CHECK(
				std::find(adjaNodes->begin(), adjaNodes->end(), node2)
						!= adjaNodes->end());
		if (counter != 0) {
			//exclude initial cycle
			adjaNodes = node2->getAdjacentNodes(true);
			BOOST_CHECK(
					std::find(adjaNodes->begin(), adjaNodes->end(), node1)
							== adjaNodes->end());
		}
		adjaNodes = node2->getAdjacentNodes(false);
		BOOST_CHECK(
				std::find(adjaNodes->begin(), adjaNodes->end(), node1)
						!= adjaNodes->end());
		std::vector<PBQP_Edge<int>*>* adjaEdge = node1->getAdjacentEdges(true);
		BOOST_CHECK(
				std::find(adjaEdge->begin(), adjaEdge->end(), edge)
						!= adjaEdge->end());
		counter++;
	}
	delete graph;
}

/*

//Generates a Kn (n nodes, each connected with each other and themselves)
PBQP_Graph<int>* genGraph(int size) {
	PBQP_Graph<int>* graph = new PBQP_Graph<int>();
	for (int i = 1; i <= size; i++) {
		Vektor<int>* vektor = new Vektor<int>(2, new int[2] { 3, 2 });
		graph->addNode(vektor);
	}
	for (PBQP_Node<int>* node1 : *(graph->getNodes())) {
		for (PBQP_Node<int>* node2 : *(graph->getNodes())) {
			Matrix<int>* matrix = new Matrix<int>(2, 2,
					new int[4] { 3, 2, 5, 8 });
			graph->addEdge(node1, node2, matrix);
		}
	}
	return graph;
}

BOOST_AUTO_TEST_CASE(advancedEdgeGeneration) {
	int size = 20;
	PBQP_Graph<int>* graph = genGraph(size);
	BOOST_CHECK_EQUAL(graph->getEdgeCount(), size * size);
	BOOST_CHECK_EQUAL(graph->getNodeCount(), size);
	for(PBQP_Node<int>* node : *(graph->getNodes())) {
		BOOST_CHECK_EQUAL(node->getDegree(), size * 2 - 1);
		BOOST_CHECK_EQUAL(node->getAdjacentNodes(true)->size(), size);
		BOOST_CHECK_EQUAL(node->getAdjacentNodes(false)->size(), size);
		BOOST_CHECK_EQUAL(node->getAdjacentEdges(false)->size(), size * 2 - 1);
		BOOST_CHECK_EQUAL(node->getAdjacentEdges(true)->size(), size);
	}
}

BOOST_AUTO_TEST_CASE(advancedEdgeRemoval) {
	int size = 20;
	PBQP_Graph<int>* graph = genGraph(size);
	PBQP_Node<int>* node = *(graph->getNodes()->begin());
	int removed = 0;
	std::vector<PBQP_Node<int>*>* adjaNodes;
	for(PBQP_Edge<int>* edge : *(node->getAdjacentEdges(true))) {
		PBQP_Node<int>* other = edge->getOtherEnd(node);
		graph->removeEdge(edge);
		removed++;
		BOOST_CHECK_EQUAL(graph->getEdgeCount(), size * size - removed);
		BOOST_CHECK_EQUAL(node->getDegree(), size - removed);
		BOOST_CHECK_EQUAL(other->getDegree(), size - 1);
		adjaNodes = other->getAdjacentNodes(false);
		BOOST_CHECK(
						std::find(adjaNodes->begin(), adjaNodes->end(), node)
								== adjaNodes->end());
		adjaNodes = node->getAdjacentNodes(false);
		BOOST_CHECK(
						std::find(adjaNodes->begin(), adjaNodes->end(), other)
								== adjaNodes->end());
	}

} */
