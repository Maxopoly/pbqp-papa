#ifndef REDUCTION_SELFCYCLEREMOVER_HPP_
#define REDUCTION_SELFCYCLEREMOVER_HPP_

#include <vector>

namespace pbqppapa {

template<typename T>
class NodeConsistentReduction;
template<typename T>
class PBQPGraph;
template<typename T>
class PBQPSolution;

template<typename T>
class SelfCycleRemover: public PBQP_Reduction<T> {
public:
	SelfCycleRemover(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	std::vector<PBQPGraph<T>*>* reduce() override {
		auto iter = this->graph->getEdgeBegin();
		while (iter != this->graph->getEdgeEnd()) {
			PBQPEdge<T>* edge = *iter;
			if (edge->getSource() == edge->getTarget()) {
				//add diagonal of edge matrix to cost vektor and then delete edge
				for (unsigned short int i = 0;
						i < edge->getSource()->getVektorDegree(); i++) {
					edge->getSource()->getVektor()->get(i) +=
							edge->getMatrix()->get(i, i);
				}
				++iter;
				this->graph->removeEdge(edge);
			} else {
				++iter;
			}
		}
		this->result->push_back(this->graph);
		return this->result;
	}

	PBQPSolution<T>* solve(PBQPSolution<T>* solution) override {
		//don't need to do anything
		return solution;
	}
};

}

#endif /* REDUCTION_SELFCYCLEREMOVER_HPP_ */
