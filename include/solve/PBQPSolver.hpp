#ifndef SOLVE_PBQPSOLVER_HPP_
#define SOLVE_PBQPSOLVER_HPP_

#include "analysis/PBQPHandler.hpp"
#include "analysis/PBQPHandler.hpp"
#include "graph/PBQPSolution.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;

template<typename T>
class PBQPSolution;

template<typename T>
class PBQPHandler;

/**
 * Interface for a class solving a PBQP.
 *
 * Opposed to reductions which change the graph, solvers are supposed to fill
 * the given solution object with a solution for every node in the given graph
 *
 * How exactly this solution is determined and how good it is, is completely up to the implementation
 */
template<typename T>
class PBQPSolver: public PBQPHandler<T> {

protected:
	PBQPSolution<T>* const solution;

	virtual void solve() = 0;

public:
	PBQPSolver(PBQPGraph<T>* graph) :
			PBQPHandler<T>(graph), solution(new PBQPSolution<T>(graph->getNodeIndexCounter())) {
	}

	virtual ~PBQPSolver() {}

	PBQPSolution<T>* calcSolution() {
		if (this->graph->getNodeCount() != 0) {
			solve();
		}
		return solution;
	}

};

}

#endif /* SOLVE_PBQPSOLVER_HPP_ */
