#ifndef ANALYSIS_MINIMALSOLUTIONCHECKER_HPP_
#define ANALYSIS_MINIMALSOLUTIONCHECKER_HPP_

template<typename T>
class PBQP_Graph;
template<typename T>
class PBQPHandler;

/**
 * Calculates the theoretically possible minimum for a given graph, assuming we chose the minimum for every matrix and every vektor
 */
template<typename T>
class MinimalSolutionChecker : PBQPHandler<T>{

public:
	MinimalSolutionChecker(PBQP_Graph<T>* graph) : PBQPHandler<T>(graph) {
	}

	 ~MinimalSolutionChecker() {

	 }

	 T getMinimalSolution() {
		 T value = new T();
		 //TODO
	 }

};



#endif /* ANALYSIS_MINIMALSOLUTIONCHECKER_HPP_ */
