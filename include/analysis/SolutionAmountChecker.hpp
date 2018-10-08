#ifndef ANALYSIS_SOLUTIONAMOUNTCHECKER_HPP_
#define ANALYSIS_SOLUTIONAMOUNTCHECKER_HPP_

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPHandler;
template<typename T>
class PBQPNode;

/**
 * Calculates how many different solution selection are theoretically possible
 */
template<typename T>
class SolutionAmountChecker: PBQPHandler<T> {

public:
	SolutionAmountChecker(PBQPGraph<T>* graph) :
			PBQPHandler<T>(graph) {
	}

	~SolutionAmountChecker() {
	}

	unsigned long long int getSolutionAmount() {
		unsigned long long amount = 1;
		for (auto iter = this->graph->getNodeBegin(); iter != this->graph->getNodeEnd(); ++iter) {
			amount *= (*iter)->getVectorDegree();
			std::cout << std::to_string(amount) << "   ";
		}
		return amount;
	}

};

}

#endif /* ANALYSIS_SOLUTIONAMOUNTCHECKER_HPP_ */
