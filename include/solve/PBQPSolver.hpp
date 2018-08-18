#ifndef SOLVE_PBQPSOLVER_HPP_
#define SOLVE_PBQPSOLVER_HPP_

template<typename T>
class PBQP_Graph;

template<typename T>
class PBQP_Solution;

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
class PBQPSolver : PBQPHandler<T>{

protected:
	PBQP_Solution<T>* const solution;
	virtual PBQP_Solution<T> solve() = 0;

public:
	PBQPSolver(PBQP_Graph<T>* graph, PBQP_Solution<T>* solution) : PBQPHandler<T>(graph), solution(solution) {
	}

	virtual ~PBQPSolver();

	PBQP_Solution<T> getSolution() {
		if (graph->getNodeCount() == 0) {
			return solution;
		}
		return solve();
	}

};


#endif /* SOLVE_PBQPSOLVER_HPP_ */
