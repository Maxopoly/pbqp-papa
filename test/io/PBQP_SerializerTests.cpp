
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PBQP_SerializerTests
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <algorithm>

#include "graph/PBQP_Graph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"
#include "io/PBQP_Serializer.hpp"
#include "io/types/UnsignedLongIntSerializer.hpp"


#include "util/TestUtils.hpp"

BOOST_AUTO_TEST_CASE(dummyTest) {
	PBQP_Graph<unsigned long int>* graph = new PBQP_Graph<unsigned long int>();
	Vektor<unsigned long int>* vek1 = new Vektor<unsigned long int>(3, new unsigned long int[3] { 1, 2, 3 });
	PBQP_Node<unsigned long int>* node1 = graph->addNode(vek1);
	Vektor<unsigned long int>* vek2 = new Vektor<unsigned long int>(2, new unsigned long int[2] { 4, 5 });
	PBQP_Node<unsigned long int>* node2 = graph->addNode(vek2);
	Vektor<unsigned long int>* vek3 = new Vektor<unsigned long int>(3, new unsigned long int[3] { 6, 7, 8 });
	PBQP_Node<unsigned long int>* node3 = graph->addNode(vek3);
	//normal edges
	graph->addEdge(node1, node2, new Matrix<unsigned long int>(3, 2, new unsigned long int[6] { 5, 5, 5, 5,
			5, 5 }));
	graph->addEdge(node2, node3, new Matrix<unsigned long int>(2, 3, new unsigned long int[6] { 5, 5, 5, 5,
			5, 5 }));
	graph->addEdge(node3, node1, new Matrix<unsigned long int>(3, 3, new unsigned long int[9] { 5, 5, 5, 5,
			5, 5, 5, 5, 5 }));
	//cycles
	graph->addEdge(node1, node1, new Matrix<unsigned long int>(3, 3, new unsigned long int[9] { 9, 10, 11, 12,
			13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1, new Matrix<unsigned long int>(3, 3, new unsigned long int[9] { 9, 10, 11, 12,
			13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1, new Matrix<unsigned long int>(3, 3, new unsigned long int[9] { 9, 10, 11, 12,
			13, 14, 15, 16, 17 }));
	graph->addEdge(node2, node2, new Matrix<unsigned long int>(2, 2, new unsigned long int[4] { 20, 21, 22, 23}));

	PBQP_Serializer<unsigned long int> serial;
	serial.saveToFile("testgraph.json", graph, new UnsignedLongIntSerializer ());


}
