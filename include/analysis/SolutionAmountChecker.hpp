#ifndef ANALYSIS_SOLUTIONAMOUNTCHECKER_HPP_
#define ANALYSIS_SOLUTIONAMOUNTCHECKER_HPP_

template<typename T>
class PBQP_Graph;
template<typename T>
class PBQPHandler;
template<typename T>
class PBQP_Node;

/**
 * Calculates how many different solution selection are theoretically possible
 */
template<typename T>
class SolutionAmountChecker: PBQPHandler<T> {

public:
	SolutionAmountChecker(PBQP_Graph<T>* graph) :
			PBQPHandler<T>(graph) {
	}

	~SolutionAmountChecker() {
	}

	unsigned long long int getSolutionAmount() {
		unsigned long long int amount = 1;
		for (PBQP_Node<T>* node : graph->getNodes()) {
			amount *= node->getVektorDegree();
		}
		return amount;
	}

};

#endif /* ANALYSIS_SOLUTIONAMOUNTCHECKER_HPP_ */
