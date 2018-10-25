#ifndef REDUCTION_VectorDEGREEZEROREDUCTOR_HPP_
#define REDUCTION_VectorDEGREEZEROREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class NtoNDependentSolution;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;

/**
 * Any nodes with a Vector degree of 0 can be deleted from the graph. No selection needs to be made for them
 */
template<typename T>
class VectorDegreeZeroReducer: public PBQP_Reduction<T> {
private:

public:
	VectorDegreeZeroReducer(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	~VectorDegreeZeroReducer() {
	}

	std::vector<PBQPGraph<T>*>& reduce() {
		auto iter = this->graph->getNodeBegin();
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter;
			if (node->getVectorDegree() == 0) {
				reduceVectorDegreeZero(node, this->graph);
			}
		}
		this->result->push_back(this->graph);
		return this->result;
	}

	void solve(PBQPSolution<T>* solution) {

	}

	/**
	 * Deletes the node as there is nothing to solve
	 */
	static void reduceVectorDegreeZero(PBQPNode<T>* node, PBQPGraph<T>* graph) {
		//TODO: This leaves uninitialized values in the solution. How do we want to deal with unsolvable nodes in general?
		graph->removeNode(node);
	}
};

}

#endif /* REDUCTION_VectorDEGREEZEROREDUCTOR_HPP_ */
