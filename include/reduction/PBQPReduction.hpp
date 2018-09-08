#ifndef REDUCTION_PBQPREDUCTION_HPP_
#define REDUCTION_PBQPREDUCTION_HPP_

#include <vector>

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPSolution;

/**
 * Simplifies a PBQP by removing a part of it.
 *
 * A reduction takes a good look at a PBQP to determine a subset A of the PBQP P, where P could be solved perfectly,
 * if a solution for P \ A was known. The reduction removes A from P and passes P \ A on to further reductions or a solver.
 *
 * Later on a solution for P \ A is given to the reduction instance and the instance is expected to add to the solution,
 * so it solves P completly
 *
 * The overall goal here is to simplify the PBQP for the final solving step, how exactly that is achieved is completly up
 * to the implementation. Adding/removing nodes, adding/removing edges and modifying cost matrices/vektors is all allowed.
 *
 */
template<typename T>
class PBQP_Reduction {
protected:
	PBQPGraph<T>* graph;
	std::vector<PBQPGraph<T>*>* result;

public:
	/**
	 * Initiates a new reduction for the given graph. The given graph may be deconstructed/modified
	 * completly or passed on as a result of the reduction. It is not needed anywhere else and this instance
	 * has full power over it. If the graph isnt passed on, it must be deleted by the reduction.
	 */
	PBQP_Reduction(PBQPGraph<T>* graph) {
		this->graph = graph;
		result = new std::vector<PBQPGraph<T>*>();
	}
	virtual ~PBQP_Reduction()  {

	}

	/**
	 * Applies whatever logic this instance implements to simplify the PBQP. Result is a vector to allow splitting
	 * up the graph into multiple problems
	 */
	virtual std::vector<PBQPGraph<T>*>* reduce() = 0;

	/**
	 * Backtracking step. The given solution contains a solution for all nodes contained in any of the graphs returned
	 * by reduce() and this instance is expected to further complete the solution with anything that was reduced as part
	 * of this step
	 */
	virtual PBQPSolution<T>* solve(PBQPSolution<T>* solution) = 0;
};

#endif /* REDUCTION_PBQPREDUCTION_HPP_ */
