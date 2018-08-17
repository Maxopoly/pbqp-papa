
#include <stdlib.h>
#include <algorithm>

#include "graph/PBQP_Graph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"

PBQP_Graph<double>* genGraph(int size) {
	PBQP_Graph<double>* graph = new PBQP_Graph<double>();
	for (int i = 1; i <= size; i++) {
		Vektor<double>* vektor = new Vektor<double>(2, new double[2] { 3.0, 2.0 });
		graph->addNode(vektor);
	}
	for (PBQP_Node<double>* node1 : *(graph->getNodes())) {
		for (PBQP_Node<double>* node2 : *(graph->getNodes())) {
			Matrix<double>* matrix = new Matrix<double>(2, 2,
					new double[4] { 3.0, 2.0, 5.4, 8.6 });
			graph->addEdge(node1, node2, matrix);
		}
	}
	return graph;
}


int main() {
	int size = 20;
	PBQP_Graph<double>* graph = genGraph(size);
	(graph->getEdgeCount(), size * size);
	(graph->getNodeCount(), size);
	for(PBQP_Node<double>* node : *(graph->getNodes())) {
		(node->getDegree(), size * 2 - 1);
		(node->getAdjacentNodes(true)->size(), size);
		(node->getAdjacentNodes(false)->size(), size);
		(node->getAdjacentEdges(false)->size(), size * 2 - 1);
		(node->getAdjacentEdges(true)->size(), size);
	}
}

void test1() {
	PBQP_Graph<int>* graph = new PBQP_Graph<int>();

	//generate a bunch of nodes
	for (int i = 1; i <= 50; i++) {
		Vektor<int>* vektor = new Vektor<int>(2, new int[2] { 3, 2 });
		PBQP_Node<int>* node = graph->addNode(vektor);
	}

	//generate a bunch of edges
	PBQP_Node<int>* node1 = *(graph->getNodes()->begin());
	std::set<PBQP_Node<int>*>* nodes = graph->getNodes();
	int counter = 0;
	for (std::set<PBQP_Node<int>*>::iterator it = nodes->begin(); it != nodes->end(); it++) {
		Matrix<int>* matrix = new Matrix<int>(2, 2, new int[4] { 3, 2, 5, 8 });
		PBQP_Node<int>* node2 = *it;
		std::vector<PBQP_Node<int>*>* adjaNodes = node1->getAdjacentNodes(true);
				std::find(adjaNodes->begin(), adjaNodes->end(), node2)
						== adjaNodes->end();
		PBQP_Edge<int>* edge = graph->addEdge(node1, node2, matrix);
		adjaNodes = node1->getAdjacentNodes(true);
				std::find(adjaNodes->begin(), adjaNodes->end(), node2)
						!= adjaNodes->end();
		adjaNodes = node1->getAdjacentNodes(false);
				std::find(adjaNodes->begin(), adjaNodes->end(), node2)
						!= adjaNodes->end();
		if (counter != 0) {
			//exclude initial cycle
			adjaNodes = node2->getAdjacentNodes(true);
					std::find(adjaNodes->begin(), adjaNodes->end(), node1)
							== adjaNodes->end();
		}
		adjaNodes = node2->getAdjacentNodes(false);

				std::find(adjaNodes->begin(), adjaNodes->end(), node1)
						!= adjaNodes->end();
		std::vector<PBQP_Edge<int>*>* adjaEdge = node1->getAdjacentEdges(true);
				std::find(adjaEdge->begin(), adjaEdge->end(), edge)
						!= adjaEdge->end();
		counter++;
	}
	delete graph;
}
