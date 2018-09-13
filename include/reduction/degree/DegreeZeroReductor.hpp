#ifndef VALIDATION_DEGREEZEROREDUCTOR_HPP_
#define VALIDATION_DEGREEZEROREDUCTOR_HPP_

#include <vector>
#include "reduction/PBQPReduction.hpp"
#include "reduction/DependentSolution.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class DependentSolution;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;

template<typename T>
class DegreeZeroReductor: public PBQP_Reduction<T> {
private:
	DependentSolution<T>* solution;
	//we need this a lot, so keeping it around instead of recreating is good
	static const std::vector<unsigned short int> emptyIntVector;

public:
	DegreeZeroReductor(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph), solution(0) {
	}

	~DegreeZeroReductor() {
		delete solution;
	}

	std::vector<PBQPGraph<T>*>& reduce() override {
		std::vector<PBQPNode<T>*> targetNodes = std::vector<PBQPNode<T>*>();
		std::vector<unsigned short int> nodeSolution = std::vector<unsigned short int>();
		auto iter = this->graph->getNodeBegin();
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter;
			iter++;
			if (node->getDegree() == 0) {
				nodeSolution.push_back(node->getVector().getIndexOfSmallestElement());
				targetNodes.push_back(node);
				this->graph->removeNode(node);
			}
		}
		solution = new DependentSolution<T>(std::vector<PBQPNode<T>*>(0), targetNodes);
		solution->setSolution(emptyIntVector, nodeSolution);
		this->result.push_back(this->graph);
		return this->result;
	}

	void solve(PBQPSolution<T>& solution) override {
		this->solution->solve(solution);
	}

	/**
	 * Reduces a given node of degree 0. Useful when combining reductions working
	 * on different degrees to save overhead of creating lots of reduction instances
	 */
	static DependentSolution<T>* reduceDegreeZero(PBQPNode<T>* node,
			PBQPGraph<T>* graph) {
		std::vector<PBQPNode<T>*> dependencyNodes = std::vector<PBQPNode<T>*>();
		std::vector<PBQPNode<T>*> solutionNodes = std::vector<PBQPNode<T>*>();
		solutionNodes.push_back(node);
		DependentSolution<T>* solution = new DependentSolution<T>(dependencyNodes, solutionNodes);
		std::vector<unsigned short int> nodeSolution = std::vector<unsigned short int>();
		nodeSolution.push_back(node->getVector().getIndexOfSmallestElement());
		solution->setSolution(emptyIntVector, nodeSolution);
		return solution;
	}
};

//dark magic to initialize static members of a template
template<typename T>
const std::vector<unsigned short int> DegreeZeroReductor<T>::emptyIntVector = std::vector<unsigned short int>(0);
}

#endif /* VALIDATION_DEGREEZEROREDUCTOR_HPP_ */
