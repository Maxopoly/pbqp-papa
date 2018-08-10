/*
 * SelfCycleRemover.cpp
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#include "reduction/SelfCycleRemover.hpp"

#include "graph/PBQP_Graph.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Edge.hpp"
#include "graph/Vektor.hpp"

SelfCycleRemover::SelfCycleRemover(PBQP_Graph* graph) :
		PBQP_Reduction(graph) {
}

std::vector<PBQP_Graph*>* SelfCycleRemover::reduce() {
	for(int i = 0; i < graph->getEdgeCount(); i++) {
		PBQP_Edge* edge = (*(graph->getEdges())) [i];
		if(edge->getSource() == edge->getTarget()) {
			*(edge->getSource()->getVektor()) +=  *(edge->getMatrix()->getDiagonal());
			graph->removeEdge(edge);
			i--;
		}
	}
	result->push_back(graph);
	return result;
}


PBQP_Solution* SelfCycleRemover::solve(PBQP_Solution* solution) {
	//we dont actually modify any nodes
	return solution;
}
