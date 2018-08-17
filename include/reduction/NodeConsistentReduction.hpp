#ifndef REDUCTION_NODECONSISTENTREDUCTION_HPP_
#define REDUCTION_NODECONSISTENTREDUCTION_HPP_

#include <vector>

template<typename T>
class PBQP_Reduction;

template<typename T>
class PBQP_Graph;

/**
 * Reduction that doesnt actually remove or add any nodes,
 * so we can skip the solving step during backtracking
 */
template<typename T>
class NodeConsistentReduction: public PBQP_Reduction<T> {

	NodeConsistentReduction(PBQP_Graph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	std::vector<PBQP_Graph<T>*>* reduce() = 0;

	PBQP_Solution<T>* solve(PBQP_Solution<T>* solution) {
		//don't need to do anything
		return solution;
	}

};

#endif /* REDUCTION_NODECONSISTENTREDUCTION_HPP_ */
