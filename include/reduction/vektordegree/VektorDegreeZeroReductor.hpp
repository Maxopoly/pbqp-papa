#ifndef REDUCTION_VEKTORDEGREEZEROREDUCTOR_HPP_
#define REDUCTION_VEKTORDEGREEZEROREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>

template<typename T>
class PBQPGraph;
template<typename T>
class Dependent_Solution;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;

/**
 * Any nodes with a vektor degree of 0 can be deleted from the graph. No selection needs to be made for them
 */
template<typename T>
class VektorDegreeZeroReductor: public PBQP_Reduction<T> {
private:

public:
	VektorDegreeZeroReductor(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	~VektorDegreeZeroReductor() {
	}

	std::vector<PBQPGraph*>* reduce() {
		for (PBQPNode<T>* node : *(graph->getNodes())) {
			if (node->getVektorDegree() == 0) {
				reduceVektorDegreeZero(node, graph);
			}
		}
		result->push_back(graph);
		return result;
	}

	PBQPSolution<T>* solve(PBQPSolution<T>* solution) {
		return solution;
	}

	/**
	 * Deletes the node as there is nothing to solve
	 */
	static void reduceVektorDegreeZero(PBQPNode<T>* node, PBQPGraph<T>* graph) {
		//TODO: This leaves uninitialized values in the solution. How do we want to deal with unsolvable nodes in general?
		graph->removeNode(node);
	}
};

#endif /* REDUCTION_VEKTORDEGREEZEROREDUCTOR_HPP_ */
