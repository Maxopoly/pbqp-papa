/*
 * SelfCycleRemover.h
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#ifndef REDUCTION_SELFCYCLEREMOVER_HPP_
#define REDUCTION_SELFCYCLEREMOVER_HPP_

#include <vector>

template<typename T>
class NodeConsistentReduction;
template<typename T>
class PBQP_Graph;
template<typename T>
class PBQP_Solution;

template<typename T>
class SelfCycleRemover : public NodeConsistentReduction <T>{
public:
	SelfCycleRemover(PBQP_Graph<T>*) : NodeConsistentReduction<T>(graph) {
	}

	std::vector<PBQP_Graph<T>*>* reduce() {
		for(int i = 0; i < graph->getEdgeCount(); i++) {
			PBQP_Edge<T>* edge = (*(graph->getEdges())) [i];
			if(edge->getSource() == edge->getTarget()) {
				*(edge->getSource()->getVektor()) +=  *(edge->getMatrix()->getDiagonal());
				graph->removeEdge(edge);
				i--;
			}
		}
		result->push_back(graph);
		return result;
	}
};

#endif /* REDUCTION_SELFCYCLEREMOVER_HPP_ */
