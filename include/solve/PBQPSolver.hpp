#ifndef SOLVE_PBQPSOLVER_HPP_
#define SOLVE_PBQPSOLVER_HPP_

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
class PBQPSolver: PBQPHandler<T> {

protected:
	PBQPSolution<T>* const solution;

	virtual void solve() = 0;

public:
	PBQPSolver(PBQPGraph<T>* graph, PBQPSolution<T>* solution) :
			PBQPHandler<T>(graph), solution(solution) {
	}

	virtual ~PBQPSolver();

	void calcSolution() {
		if (graph->getNodeCount() == 0) {
			return;
		}
		solve();
	}

};

}

#endif /* SOLVE_PBQPSOLVER_HPP_ */
