/*
 * SelfCycleRemover.cpp
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#include "SelfCycleRemover.hpp"

#include "PBQP_Graph.hpp";
#include "PBQP_Node.hpp";
#include "PBQP_Edge.hpp";
#include "Matrix.hpp";
#include <vector>
#include "PBQPReduction.hpp"

SelfCycleRemover::SelfCycleRemover(PBQP_Graph* graph) :
		PBQP_Reduction(graph) {
}

PBQP_Graph* SelfCycleRemover::solve() {
	for(int i = 0; i < graph->getEdgeCount(); i++) {
		PBQP_Edge* edge = *(graph->getEdges()) [i];
		if(edge->getSource() == edge->getTarget()) {
			edge->getSource()->getVektor() += edge->getMatrix()->getDiagonal();
			graph->removeEdge(edge);
			i--;
		}
	}
	return graph;
}


PBQP_Solution* SelfCycleRemover::solve(PBQP_Solution* solution) {
	//we dont actually modify any nodes
	return solution;
}
