#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PBQP_SerializerTests
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <algorithm>

#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "io/PBQP_Serializer.hpp"
#include "generate/PBQPGenerator.hpp"
#include "io/types/UnsignedLongIntSerializer.hpp"
#include "FullSolver.hpp"
#include "CInterface.h"

#include "util/TestUtils.hpp"

namespace pbqppapa {


BOOST_AUTO_TEST_CASE(dummyTest) {
	PBQPGraph<unsigned long>* graph = new PBQPGraph<unsigned long>();
	Vector<unsigned long> vek1 = Vector<unsigned long>(3,
			new unsigned long[3] { 1, 2, 3 });
	PBQPNode<unsigned long>* node1 = graph->addNode(vek1);
	Vector<unsigned long> vek2 = Vector<unsigned long>(2,
			new unsigned long[2] { 4, 5 });
	PBQPNode<unsigned long>* node2 = graph->addNode(vek2);
	Vector<unsigned long> vek3 = Vector<unsigned long>(3,
			new unsigned long[3] { 6, 7, 8 });
	PBQPNode<unsigned long>* node3 = graph->addNode(vek3);
	//normal edges
	unsigned long arr1 [] = { 5, 5, 5, 5, 5, 5 };
	Matrix<unsigned long> mat1 = Matrix<unsigned long>(3, 2, arr1);
	graph->addEdge(node1, node2, mat1);
	unsigned long arr2 [] = { 5, 5, 5, 5, 5, 5 };
	Matrix<unsigned long> mat2 = Matrix<unsigned long>(2, 3, arr2);
	graph->addEdge(node2, node3,mat2);
	unsigned long arr3 [] = { 5, 5, 5, 5, 5, 5, 5, 5, 5 };
	Matrix<unsigned long> mat3 = Matrix<unsigned long>(3, 3, arr3);
	graph->addEdge(node3, node1,mat3);
	//cycles
	unsigned long arr4 [] = { 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	Matrix<unsigned long> mat4 = Matrix<unsigned long>(3, 3, arr4);
	graph->addEdge(node1, node1,mat4);
	graph->addEdge(node1, node1,mat4);
	graph->addEdge(node1, node1,mat4);
	unsigned long arr5 [] = { 20,21, 22, 23 };
	Matrix<unsigned long> mat5 = Matrix<unsigned long>(2, 2, arr5);
	graph->addEdge(node2, node2,mat5);

	PBQP_Serializer<unsigned long> serial;
	serial.saveToFile("testgraph.json", graph);

	PBQPGraph<unsigned long>* secondGraph = serial.loadFromFile(
			"testgraph.json");
	BOOST_TEST_MESSAGE(secondGraph->getEdgeCount());

}

BOOST_AUTO_TEST_CASE(generatorTest) {
	PBQPGenerator<unsigned long> generator (200, 1.8, 100, 6, 2);
	PBQPGraph<unsigned long>* graph = generator.generate();
	PBQP_Serializer<unsigned long> serial;
	serial.saveToFile("bigTestGraph.json", graph);
	/*
	FullSolver<unsigned long> solver (graph);
	solver.solve();
	serial.saveToFile("reducedBigGraph.json", graph); */
	delete graph;
}

}
