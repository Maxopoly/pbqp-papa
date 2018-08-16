
#include <stdlib.h>
#include <algorithm>

#include "graph/PBQP_Graph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"

PBQP_Graph* genGraph(int size) {
	PBQP_Graph* graph = new PBQP_Graph();
	for (int i = 1; i <= size; i++) {
		Vektor* vektor = new Vektor(2, new double[2] { 3.0, 2.0 });
		graph->addNode(vektor);
	}
	for (PBQP_Node* node1 : *(graph->getNodes())) {
		for (PBQP_Node* node2 : *(graph->getNodes())) {
			Matrix* matrix = new Matrix(2, 2,
					new double[4] { 3.0, 2.0, 5.4, 8.6 });
			graph->addEdge(node1, node2, matrix);
		}
	}
	return graph;
}


int main() {
	int size = 20;
	PBQP_Graph* graph = genGraph(size);
	(graph->getEdgeCount(), size * size);
	(graph->getNodeCount(), size);
	for(PBQP_Node* node : *(graph->getNodes())) {
		(node->getDegree(), size * 2 - 1);
		(node->getAdjacentNodes(true)->size(), size);
		(node->getAdjacentNodes(false)->size(), size);
		(node->getAdjacentEdges(false)->size(), size * 2 - 1);
		(node->getAdjacentEdges(true)->size(), size);
	}
}

