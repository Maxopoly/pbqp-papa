#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vektor.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"

namespace pbqppapa {

//Generates a Kn (n nodes, each connected with each other and themselves)
PBQPGraph<int>* genGraph(int size) {
	PBQPGraph<int>* graph = new PBQPGraph<int>();
	for (int i = 1; i <= size; i++) {
		int vektorCost [] = { 3, 2 };
		Vektor<int> vektor = Vektor<int>(2, vektorCost);
		graph->addNode(vektor);
	}
	for (std::set<PBQPNode<int>*>::iterator it = graph->getNodeBegin();
			it != graph->getNodeEnd(); it++) {
		PBQPNode<int>* node1 = *it;
		for (std::set<PBQPNode<int>*>::iterator it2 = graph->getNodeBegin();
				it2 != graph->getNodeEnd(); it2++) {
			PBQPNode<int>* node2 = *it2;
			int matrixCost [] = { 3, 2, 5, 8 };
			Matrix<int> matrix = Matrix<int>(2, 2,
					matrixCost);
			graph->addEdge(node1, node2, matrix);
		}
	}
	return graph;
}

}
