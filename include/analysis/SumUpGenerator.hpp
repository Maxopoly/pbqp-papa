#ifndef ANALYSIS_SUMUPGENERATOR_HPP_
#define ANALYSIS_SUMUPGENERATOR_HPP_

#include <string>

#include "analysis/SolutionAmountChecker.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPHandler;
template<typename T>
class PBQPNode;

template<typename T>
class SumUpGenerator: PBQPHandler<T> {
public:
	SumUpGenerator(PBQPGraph<T>* graph) : PBQPHandler<T>(graph) {

	}

	~SumUpGenerator() {

	}


	std::string* genSumUp() {
		std::string res = new std::string();
		res += "Node count: ";
		res += this->graph->getNodeCount();
		res += ", Edge count: ";
		res += this->graph->getEdgeCount();
		res += ", Complexity: ";
		SolutionAmountChecker<T> solCheck (this->graph);
		res += solCheck.getSolutionAmount();
		return res;
	}



};

}

#endif /* ANALYSIS_SUMUPGENERATOR_HPP_ */
