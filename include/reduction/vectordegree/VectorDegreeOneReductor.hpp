#ifndef REDUCTION_VectorDEGREE_VectorDEGREEONEREDUCTOR_HPP_
#define REDUCTION_VectorDEGREE_VectorDEGREEONEREDUCTOR_HPP_

#include <vector>
#include <memory>

#include <reduction/PBQPReduction.hpp>
#include <reduction/DependentSolution.hpp>

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class DependentSolution;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;

/**
 * Any nodes with a Vector degree of 0 can be deleted from the graph. No selection needs to be made for them
 */
template<typename T>
class VectorDegreeOneReductor: public PBQP_Reduction<T> {
private:
	std::unique_ptr<DependentSolution<T>> solution;
	static const std::vector<unsigned short> emptyIntVector;

public:
	VectorDegreeOneReductor(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	~VectorDegreeOneReductor() {
	}

	std::vector<PBQPGraph<T>*>& reduce() {
		std::vector<PBQPNode<T>*> dependencyNodes;
		std::vector<PBQPNode<T>*> solutionNodes;
		for (auto iter = this->graph->getNodeBegin();
				iter != this->graph->getNodeEnd(); iter++) {
			PBQPNode<T>* node = (PBQPNode<T>*) *iter;
			if (node->getVectorDegree() == 1) {
				solutionNodes.push_back(node);
			}
		}
		solution = std::unique_ptr<DependentSolution<T>>(
				new DependentSolution<T>(dependencyNodes, solutionNodes));
		std::vector<unsigned short> dependencySelection;
		std::vector<unsigned short> solutionSelection(solutionNodes.size(),
				0);
		solution->setSolution(dependencySelection, solutionSelection);
		for(PBQPNode<T>* node : solutionNodes) {
			this->graph->removeNode(node);
		}
		this->result->push_back(this->graph);
		return this->result;
	}

	void solve(PBQPSolution<T>* solution) {
		this->solution->solve(solution);
	}

	static DependentSolution<T>* reduceVectorDegreeOne(PBQPNode<T>* node,
			PBQPGraph<T>* graph) {
		std::vector<PBQPNode<T>*> dependencyNodes;
		std::vector<PBQPNode<T>*> solutionNodes;
		solutionNodes.push_back(node);
		DependentSolution<T>* solution = new DependentSolution<T>(
				dependencyNodes, solutionNodes);
		std::vector<unsigned short> nodeSolution (1, 0);
		solution->setSolution(emptyIntVector, nodeSolution);
		return solution;
	}
};

template<typename T>
const std::vector<unsigned short> VectorDegreeOneReductor<T>::emptyIntVector = std::vector<unsigned short>(0);
}

#endif /* REDUCTION_VectorDEGREE_VectorDEGREEONEREDUCTOR_HPP_ */
