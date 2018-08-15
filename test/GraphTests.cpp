#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GraphTests
#include <boost/test/unit_test.hpp>
#include <stdlib.h>

#include "graph/PBQP_Graph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"

BOOST_AUTO_TEST_CASE(basicGraphTests) {
	PBQP_Graph* graph = new PBQP_Graph();
	BOOST_CHECK_EQUAL(graph->getEdgeCount(), 0);
	BOOST_CHECK_EQUAL(graph->getNodeCount(), 0);


	for(int i = 1; i <= 50; i++) {
		Vektor* vektor = new Vektor(1, new double[] { 3.0, 2.0 });
		PBQP_Node* node = graph->addNode(vektor);
		BOOST_CHECK_EQUAL(graph->getEdgeCount(), 0);
		BOOST_CHECK_EQUAL(graph->getNodeCount(), i);
		BOOST_CHECK_EQUAL(node->getIndex(), i - 1);
		BOOST_CHECK_EQUAL(node->getVektor(), vektor);
		BOOST_CHECK_EQUAL(node->getDegree(), 0);
		BOOST_CHECK_EQUAL(node->getVektorDegree(), 2);
	}

	for(int i = 1; i <= 200; i++) {
		Matrix* matrix = new Matrix(2, 2, new double[] { 3.0, 2.0, 5.4, 8.6 });
		PBQP_Node* node1 = (*(graph->getNodes())) [rand() % 100];
		PBQP_Node* node2 = (*(graph->getNodes())) [rand() % 100];
		int oldDegree1 = node1->getDegree();
		PBQP_Edge* egde = graph->addEdge(node1, node2, matrix);
		BOOST_CHECK_EQUAL(graph->getEdgeCount(), i);
		BOOST_CHECK_EQUAL(graph->getNodeCount(), 50);
		BOOST_CHECK_EQUAL();
	}


	BOOST_CHECK_EQUAL(graph->getEdgeCount(), 1);
	BOOST_CHECK_EQUAL(graph->getNodeCount(), 102);
}
