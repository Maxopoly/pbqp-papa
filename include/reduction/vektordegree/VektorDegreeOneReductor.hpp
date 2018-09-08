#ifndef REDUCTION_VEKTORDEGREE_VEKTORDEGREEONEREDUCTOR_HPP_
#define REDUCTION_VEKTORDEGREE_VEKTORDEGREEONEREDUCTOR_HPP_

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
class VektorDegreeOneReductor: public PBQP_Reduction<T> {
private:
	std::vector<unsigned int> nodes;

public:
	VektorDegreeOneReductor(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
		nodes = *new std::vector<int>();
	}

	~VektorDegreeOneReductor() {
	}

	std::vector<PBQPGraph*>* reduce() {
		std::vector<PBQPNode*> dependencyNodes = *new std::vector<PBQPNode*>();
		std::vector<PBQPNode*> solutionNodes = *new std::vector<PBQPNode*>();
		for (PBQPNode<T>* node : *(graph->getNodes())) {
			if (node->getVektorDegree() == 1) {
				solutionNodes.push_back(node);
			}
		}
		result->push_back(graph);
		return result;
	}

	PBQPSolution<T>* solve(PBQPSolution<T>* solution) {
		return solution;
	}

	static DependentSolution reduceVektorDegreeOne(PBQPNode<T>* node,
			PBQPGraph<T>* graph) {
		std::vector<PBQPNode*> dependencyNodes = *new std::vector<PBQPNode*>();
		std::vector<PBQPNode*> solutionNodes = *new std::vector<PBQPNode*>();
		solutionNodes.push_back(node);
		Dependent_Solution<T>* solution = new Dependent_Solution<T>(
				dependencyNodes, solutionNodes);
		std::vector<unsigned short int> nodeSolution = new std::vector<
				unsigned short int>();
		nodeSolution.push_back(0);
		solution->setSolution(&emptyIntVector, &nodeSolution);
		return solution;
	}
};

#endif /* REDUCTION_VEKTORDEGREE_VEKTORDEGREEONEREDUCTOR_HPP_ */
