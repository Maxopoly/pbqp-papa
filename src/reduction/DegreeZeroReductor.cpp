/*
 * DegreeZeroReductor.cpp
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#include "reduction/DegreeZeroReductor.hpp"

#include "PBQP_Graph.hpp";
#include "PBQP_Node.hpp";
#include "PBQP_Edge.hpp";
#include "Matrix.hpp";
#include <map>
#include <vector>
#include "PBQPReduction.hpp"
#include "Vektor.hpp"
#include "Dependent_Solution.hpp"


DegreeZeroReductor::DegreeZeroReductor(PBQP_Graph* graph) :
		PBQP_Reduction(graph) {
}

std::vector<PBQP_Graph*>* DegreeZeroReductor::reduce() {
	std::vector <PBQP_Node*>* nodes = graph->getNodes();
	std::vector <int>* nodeIndices = new std::vector<int>();
	std::vector<int>* nodeSolution = new std::vector<int>();
	for(int i = 0; i < graph->getNodeCount(); i++) {
		if(((*nodes) [i])->getDegree() == 0) {
			PBQP_Node* node = ((*nodes) [i]);
			nodeSolution->push_back(node->getVektor()->getIndexOfSmallestElement());
			nodeIndices->push_back(node->getIndex());
			graph->removeNode(node);
			i--;
		}
	}
	solution = new Dependent_Solution(new std::vector<PBQP_Node*>(0), nodeIndices);
	solution.setSolution(new std::vector<int>(0), nodeSolution);
	result.push_back(graph);
	return result;
}

PBQP_Solution* DegreeZeroReductor::solve(PBQP_Solution* solution) {
	this->solution.solve(solution);
	return solution;
}

