#ifndef ANALYSIS_MINIMALSOLUTIONCHECKER_HPP_
#define ANALYSIS_MINIMALSOLUTIONCHECKER_HPP_

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPHandler;

/**
 * Calculates the theoretically possible minimum for a given graph, assuming we chose the minimum for every matrix and every vektor
 */
template<typename T>
class MinimalSolutionChecker: PBQPHandler<T> {

public:
	MinimalSolutionChecker(PBQPGraph<T>* graph) :
			PBQPHandler<T>(graph) {
	}

	~MinimalSolutionChecker() {

	}

	T getMinimalSolution() {
		T value = new T();
		//TODO
	}

};

}

#endif /* ANALYSIS_MINIMALSOLUTIONCHECKER_HPP_ */
