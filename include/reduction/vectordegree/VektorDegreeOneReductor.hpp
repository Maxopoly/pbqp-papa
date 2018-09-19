#ifndef REDUCTION_VectorDEGREE_VectorDEGREEONEREDUCTOR_HPP_
#define REDUCTION_VectorDEGREE_VectorDEGREEONEREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>

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
	std::vector<unsigned int> nodes;

public:
	VectorDegreeOneReductor(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
		nodes = *new std::vector<int>();
	}

	~VectorDegreeOneReductor() {
	}

	std::vector<PBQPGraph*>* reduce() {
		std::vector<PBQPNode*> dependencyNodes = *new std::vector<PBQPNode*>();
		std::vector<PBQPNode*> solutionNodes = *new std::vector<PBQPNode*>();
		for (PBQPNode<T>* node : *(graph->getNodes())) {
			if (node->getVectorDegree() == 1) {
				solutionNodes.push_back(node);
			}
		}
		result->push_back(graph);
		return result;
	}

	PBQPSolution<T>* solve(PBQPSolution<T>* solution) {
		return solution;
	}

	static DependentSolution reduceVectorDegreeOne(PBQPNode<T>* node,
			PBQPGraph<T>* graph) {
		std::vector<PBQPNode*> dependencyNodes = *new std::vector<PBQPNode*>();
		std::vector<PBQPNode*> solutionNodes = *new std::vector<PBQPNode*>();
		solutionNodes.push_back(node);
		DependentSolution<T>* solution = new DependentSolution<T>(
				dependencyNodes, solutionNodes);
		std::vector<unsigned short int> nodeSolution = new std::vector<
				unsigned short int>();
		nodeSolution.push_back(0);
		solution->setSolution(&emptyIntVector, &nodeSolution);
		return solution;
	}
};

}

#endif /* REDUCTION_VectorDEGREE_VectorDEGREEONEREDUCTOR_HPP_ */
