#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"

namespace pbqppapa {

//Generates a Kn (n nodes, each connected with each other and themselves)
PBQPGraph<int>* genGraph(unsigned int size) {
	PBQPGraph<int>* graph = new PBQPGraph<int>();
	for (unsigned int i = 1; i <= size; i++) {
		int arr1[] = {3, 2};
		Vector<int> vector = Vector<int>(2, arr1);
		graph->addNode(vector);
	}
	for (std::set<PBQPNode<int>*>::iterator it = graph->getNodeBegin();
			it != graph->getNodeEnd(); it++) {
		PBQPNode<int>* node1 = *it;
		for (std::set<PBQPNode<int>*>::iterator it2 = graph->getNodeBegin();
				it2 != graph->getNodeEnd(); it2++) {
			PBQPNode<int>* node2 = *it2;
			int matrixCost[] = { 3, 2, 5, 8 };
			Matrix<int> matrix = Matrix<int>(2, 2, matrixCost);
			graph->addEdge(node1, node2, matrix);
		}
	}
	return graph;
}

}
