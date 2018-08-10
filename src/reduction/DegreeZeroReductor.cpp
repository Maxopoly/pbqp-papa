/*
 * DegreeZeroReductor.cpp
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#include "reduction/DegreeZeroReductor.hpp"

#include "graph/PBQP_Graph.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"
#include <map>
#include <vector>
#include "graph/Vektor.hpp"
#include "reduction/Dependent_Solution.hpp"

DegreeZeroReductor::DegreeZeroReductor(PBQP_Graph* graph) :
		PBQP_Reduction(graph) {
}

DegreeZeroReductor::~DegreeZeroReductor() {
	delete solution;
}

std::vector<PBQP_Graph*>* DegreeZeroReductor::reduce() {
	std::vector<PBQP_Node*>* nodes = graph->getNodes();
	std::vector<PBQP_Node*>* targetNodes = new std::vector<PBQP_Node*>();
	std::vector<int>* nodeSolution = new std::vector<int>();
	for (int i = 0; i < graph->getNodeCount(); i++) {
		if (((*nodes)[i])->getDegree() == 0) {
			PBQP_Node* node = ((*nodes)[i]);
			nodeSolution->push_back(
					node->getVektor()->getIndexOfSmallestElement());
			targetNodes->push_back(node);
			graph->removeNode(node);
			i--;
		}
	}
	solution = new Dependent_Solution(new std::vector<PBQP_Node*>(0),
			targetNodes);
	solution->setSolution(new std::vector<int>(0), nodeSolution);
	result->push_back(graph);
	return result;
}

PBQP_Solution* DegreeZeroReductor::solve(PBQP_Solution* solution) {
	this->solution->solve(solution);
	return solution;
}

