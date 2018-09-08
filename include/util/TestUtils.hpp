#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"

//Generates a Kn (n nodes, each connected with each other and themselves)
PBQPGraph<int>* genGraph(int size) {
	PBQPGraph<int>* graph = new PBQPGraph<int>();
	for (int i = 1; i <= size; i++) {
		Vektor<int>* vektor = new Vektor<int>(2, new int[2] { 3, 2 });
		graph->addNode(vektor);
	}
	for (std::set<PBQPNode<int>*>::iterator it = graph->getNodeBegin();
			it != graph->getNodeEnd(); it++) {
		PBQPNode<int>* node1 = *it;
		for (std::set<PBQPNode<int>*>::iterator it2 = graph->getNodeBegin();
				it2 != graph->getNodeEnd(); it2++) {
			PBQPNode<int>* node2 = *it2;
			Matrix<int>* matrix = new Matrix<int>(2, 2,
					new int[4] { 3, 2, 5, 8 });
			graph->addEdge(node1, node2, matrix);
		}
	}
	return graph;
}

