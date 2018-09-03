#include <vector>
#include <set>

#include "graph/PBQP_Graph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"


//Generates a Kn (n nodes, each connected with each other and themselves)
PBQP_Graph<int>* genGraph(int size) {
	PBQP_Graph<int>* graph = new PBQP_Graph<int>();
	for (int i = 1; i <= size; i++) {
		Vektor<int>* vektor = new Vektor<int>(2, new int[2] { 3, 2 });
		graph->addNode(vektor);
	}
	for (std::set<PBQP_Node<int>*>::iterator it = graph->getNodeBegin();
				it != graph->getNodeEnd(); it++) {
		PBQP_Node<int>* node1 = *it;
		for (std::set<PBQP_Node<int>*>::iterator it2 = graph->getNodeBegin();
					it2 != graph->getNodeEnd(); it2++) {
			PBQP_Node<int>* node2 = *it2;
			Matrix<int>* matrix = new Matrix<int>(2, 2,
					new int[4] { 3, 2, 5, 8 });
			graph->addEdge(node1, node2, matrix);
		}
	}
	return graph;
}

