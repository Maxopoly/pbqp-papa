#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PBQP_SerializerTests
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <algorithm>

#include "graph/PBQPGraph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "io/PBQP_Serializer.hpp"
#include "io/types/UnsignedLongIntSerializer.hpp"

#include "util/TestUtils.hpp"

BOOST_AUTO_TEST_CASE(dummyTest) {
	PBQPGraph<unsigned long int>* graph = new PBQPGraph<unsigned long int>();
	Vektor<unsigned long int>* vek1 = new Vektor<unsigned long int>(3,
			new unsigned long int[3] { 1, 2, 3 });
	PBQPNode<unsigned long int>* node1 = graph->addNode(vek1);
	Vektor<unsigned long int>* vek2 = new Vektor<unsigned long int>(2,
			new unsigned long int[2] { 4, 5 });
	PBQPNode<unsigned long int>* node2 = graph->addNode(vek2);
	Vektor<unsigned long int>* vek3 = new Vektor<unsigned long int>(3,
			new unsigned long int[3] { 6, 7, 8 });
	PBQPNode<unsigned long int>* node3 = graph->addNode(vek3);
	//normal edges
	graph->addEdge(node1, node2,
			new Matrix<unsigned long int>(3, 2, new unsigned long int[6] { 5, 5,
					5, 5, 5, 5 }));
	graph->addEdge(node2, node3,
			new Matrix<unsigned long int>(2, 3, new unsigned long int[6] { 5, 5,
					5, 5, 5, 5 }));
	graph->addEdge(node3, node1,
			new Matrix<unsigned long int>(3, 3, new unsigned long int[9] { 5, 5,
					5, 5, 5, 5, 5, 5, 5 }));
	//cycles
	graph->addEdge(node1, node1,
			new Matrix<unsigned long int>(3, 3, new unsigned long int[9] { 9,
					10, 11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1,
			new Matrix<unsigned long int>(3, 3, new unsigned long int[9] { 9,
					10, 11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1,
			new Matrix<unsigned long int>(3, 3, new unsigned long int[9] { 9,
					10, 11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node2, node2,
			new Matrix<unsigned long int>(2, 2, new unsigned long int[4] { 20,
					21, 22, 23 }));

	PBQP_Serializer<unsigned long int> serial;
	serial.saveToFile("testgraph.json", graph);

	PBQPGraph<unsigned long int>* secondGraph = serial.loadFromFile(
			"testgraph.json");
	BOOST_TEST_MESSAGE(secondGraph->getEdgeCount());
}

BOOST_AUTO_TEST_CASE(dummyTest2) {
	PBQPGraph<unsigned short int>* graph = new PBQPGraph<unsigned short int>();
	Vektor<unsigned short int>* vek1 = new Vektor<unsigned short int>(3,
			new unsigned short int[3] { 1, 2, 3 });
	PBQPNode<unsigned short int>* node1 = graph->addNode(vek1);
	Vektor<unsigned short int>* vek2 = new Vektor<unsigned short int>(2,
			new unsigned short int[2] { 4, 5 });
	PBQPNode<unsigned short int>* node2 = graph->addNode(vek2);
	Vektor<unsigned short int>* vek3 = new Vektor<unsigned short int>(3,
			new unsigned short int[3] { 6, 7, 8 });
	PBQPNode<unsigned short int>* node3 = graph->addNode(vek3);
	//normal edges
	graph->addEdge(node1, node2,
			new Matrix<unsigned short int>(3, 2, new unsigned short int[6] { 5,
					5, 5, 5, 5, 5 }));
	graph->addEdge(node2, node3,
			new Matrix<unsigned short int>(2, 3, new unsigned short int[6] { 5,
					5, 5, 5, 5, 5 }));
	graph->addEdge(node3, node1,
			new Matrix<unsigned short int>(3, 3, new unsigned short int[9] { 5,
					5, 5, 5, 5, 5, 5, 5, 5 }));
	//cycles
	graph->addEdge(node1, node1,
			new Matrix<unsigned short int>(3, 3, new unsigned short int[9] { 9,
					10, 11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1,
			new Matrix<unsigned short int>(3, 3, new unsigned short int[9] { 9,
					10, 11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1,
			new Matrix<unsigned short int>(3, 3, new unsigned short int[9] { 9,
					10, 11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node2, node2,
			new Matrix<unsigned short int>(2, 2, new unsigned short int[4] { 20,
					21, 22, 23 }));

	PBQP_Serializer<unsigned short int> serial;
	serial.saveToFile("testgraph2.json", graph);

	PBQPGraph<unsigned short int>* secondGraph = serial.loadFromFile(
			"testgraph2.json");
	BOOST_TEST_MESSAGE(secondGraph->getEdgeCount());
}

BOOST_AUTO_TEST_CASE(dummyTest3) {
	PBQPGraph<unsigned char>* graph = new PBQPGraph<unsigned char>();
	Vektor<unsigned char>* vek1 = new Vektor<unsigned char>(3,
			new unsigned char[3] { 1, 2, 3 });
	PBQPNode<unsigned char>* node1 = graph->addNode(vek1);
	Vektor<unsigned char>* vek2 = new Vektor<unsigned char>(2,
			new unsigned char[2] { 4, 5 });
	PBQPNode<unsigned char>* node2 = graph->addNode(vek2);
	Vektor<unsigned char>* vek3 = new Vektor<unsigned char>(3,
			new unsigned char[3] { 6, 7, 8 });
	PBQPNode<unsigned char>* node3 = graph->addNode(vek3);
	//normal edges
	graph->addEdge(node1, node2,
			new Matrix<unsigned char>(3, 2, new unsigned char[6] { 255, 235,
					255, 5, 5, 5 }));
	graph->addEdge(node2, node3,
			new Matrix<unsigned char>(2, 3, new unsigned char[6] { 5, 5, 5, 5,
					5, 5 }));
	graph->addEdge(node3, node1,
			new Matrix<unsigned char>(3, 3, new unsigned char[9] { 5, 5, 5, 5,
					5, 5, 5, 5, 5 }));
	//cycles
	graph->addEdge(node1, node1,
			new Matrix<unsigned char>(3, 3, new unsigned char[9] { 9, 10, 11,
					12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1,
			new Matrix<unsigned char>(3, 3, new unsigned char[9] { 9, 10, 11,
					12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1,
			new Matrix<unsigned char>(3, 3, new unsigned char[9] { 9, 10, 11,
					12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node2, node2,
			new Matrix<unsigned char>(2, 2, new unsigned char[4] { 20, 21, 22,
					23 }));

	PBQP_Serializer<unsigned char> serial;
	serial.saveToFile("testgraph3.json", graph);

	PBQPGraph<unsigned char>* secondGraph = serial.loadFromFile(
			"testgraph3.json");
	BOOST_TEST_MESSAGE(secondGraph->getEdgeCount());
}

BOOST_AUTO_TEST_CASE(dummyTest4) {
	PBQPGraph<float>* graph = new PBQPGraph<float>();
	Vektor<float>* vek1 = new Vektor<float>(3, new float[3] { 1, 2, 3 });
	PBQPNode<float>* node1 = graph->addNode(vek1);
	Vektor<float>* vek2 = new Vektor<float>(2, new float[2] { 4, 5 });
	PBQPNode<float>* node2 = graph->addNode(vek2);
	Vektor<float>* vek3 = new Vektor<float>(3, new float[3] { 6, 7, 8 });
	PBQPNode<float>* node3 = graph->addNode(vek3);
	//normal edges
	graph->addEdge(node1, node2, new Matrix<float>(3, 2, new float[6] { 5, 5, 5,
			5, 5, 5 }));
	graph->addEdge(node2, node3, new Matrix<float>(2, 3, new float[6] { 5, 5, 5,
			5, 5, 5 }));
	graph->addEdge(node3, node1, new Matrix<float>(3, 3, new float[9] { 5, 5, 5,
			5, 5, 5, 5, 5, 5 }));
	//cycles
	graph->addEdge(node1, node1, new Matrix<float>(3, 3, new float[9] { 9, 10,
			11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1, new Matrix<float>(3, 3, new float[9] { 9, 10,
			11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1, new Matrix<float>(3, 3, new float[9] { 9, 10,
			11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node2, node2, new Matrix<float>(2, 2, new float[4] { 20, 21,
			22, 23 }));

	PBQP_Serializer<float> serial;
	serial.saveToFile("testgraph4.json", graph);

	PBQPGraph<float>* secondGraph = serial.loadFromFile("testgraph4.json");
	BOOST_TEST_MESSAGE(secondGraph->getEdgeCount());
}

BOOST_AUTO_TEST_CASE(dummyTest5) {
	PBQPGraph<double>* graph = new PBQPGraph<double>();
	Vektor<double>* vek1 = new Vektor<double>(3, new double[3] { 1, 2, 3 });
	PBQPNode<double>* node1 = graph->addNode(vek1);
	Vektor<double>* vek2 = new Vektor<double>(2, new double[2] { 4, 5 });
	PBQPNode<double>* node2 = graph->addNode(vek2);
	Vektor<double>* vek3 = new Vektor<double>(3, new double[3] { 6, 7, 8 });
	PBQPNode<double>* node3 = graph->addNode(vek3);
	//normal edges
	graph->addEdge(node1, node2, new Matrix<double>(3, 2, new double[6] { 5, 5,
			5, 5, 5, 5 }));
	graph->addEdge(node2, node3, new Matrix<double>(2, 3, new double[6] { 5, 5,
			5, 5, 5, 5 }));
	graph->addEdge(node3, node1, new Matrix<double>(3, 3, new double[9] { 5, 5,
			5, 5, 5, 5, 5, 5, 5 }));
	//cycles
	graph->addEdge(node1, node1, new Matrix<double>(3, 3, new double[9] { 9, 10,
			11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1, new Matrix<double>(3, 3, new double[9] { 9, 10,
			11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node1, node1, new Matrix<double>(3, 3, new double[9] { 9, 10,
			11, 12, 13, 14, 15, 16, 17 }));
	graph->addEdge(node2, node2, new Matrix<double>(2, 2, new double[4] { 20,
			21, 22, 23 }));

	PBQP_Serializer<double> serial;
	serial.saveToFile("testgraph5.json", graph);

	PBQPGraph<double>* secondGraph = serial.loadFromFile("testgraph5.json");
	BOOST_TEST_MESSAGE(secondGraph->getEdgeCount());
}
