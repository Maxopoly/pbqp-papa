#ifndef VALIDATION_DEGREEZEROREDUCTOR_HPP_
#define VALIDATION_DEGREEZEROREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>

template<typename T>
class PBQP_Graph;
template<typename T>
class Dependent_Solution;
template<typename T>
class PBQP_Solution;
template<typename T>
class PBQP_Node;

template<typename T>
class DegreeZeroReductor: public PBQP_Reduction<T> {
private:
	Dependent_Solution<T> solution;
	//we need this a lot, so keeping it around instead of recreating is good
	static const std::vector<unsigned short int> emptyIntVector = *new std::vector<unsigned short int>(0);

public:
	DegreeZeroReductor(PBQP_Graph<T>* graph) :
			PBQP_Reduction<T>(graph) {
		//solution is initialized by the reduction
	}

	~DegreeZeroReductor() {
	}

	std::vector<PBQP_Graph*>* reduce() {
		std::vector<PBQP_Node<T>*> targetNodes =
				*new std::vector<PBQP_Node<T>*>();
		std::vector<int> nodeSolution = new std::vector<int>();
		for (PBQP_Node<T>* node : *(graph->getNodes())) {
			if (node->getDegree() == 0) {
				nodeSolution.push_back(
						node->getVektor()->getIndexOfSmallestElement());
				targetNodes.push_back(node);
				graph->removeNode(node);
			}
		}
		solution = *new Dependent_Solution<T>(new std::vector<PBQP_Node*>(0),
				&targetNodes);
		solution.setSolution(&emptyIntVector, &nodeSolution);
		result->push_back(graph);
		return result;
	}

	PBQP_Solution<T>* solve(PBQP_Solution<T>* solution) {
		this->solution->solve(solution);
		return solution;
	}

	/**
	 * Reduces a given node of degree 0. Useful when combining reductions working
	 * on different degrees to save overhead of creating lots of reduction instances
	 */
	static Dependent_Solution<T>* reduceDegreeOne(PBQP_Node<T>* node,
			PBQP_Graph<T>* graph) {
		std::vector<PBQP_Node*> dependencyNodes =
				*new std::vector<PBQP_Node*>();
		std::vector<PBQP_Node*> solutionNodes = *new std::vector<PBQP_Node*>();
		solutionNodes.push_back(node);
		Dependent_Solution<T>* solution = new Dependent_Solution<T>(
				dependencyNodes, solutionNodes);
		std::vector<int> nodeSolution = new std::vector<int>();
		nodeSolution.push_back(node->getVektor()->getIndexOfSmallestElement());
		solution->setSolution(&emptyIntVector, &nodeSolution);
		return solution;
	}
};

#endif /* VALIDATION_DEGREEZEROREDUCTOR_HPP_ */
